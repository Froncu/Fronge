#include "froch.hpp"

#include "SVGParser.hpp"

namespace fro
{
	std::vector<Polygon<double>> fro::SVGParser::parse(std::string_view const filePath)
	{
		std::ifstream svgStream{ filePath.data() };

		if (!svgStream)
		{
			Logger::warn("failed to load vertices from \"{}\"", filePath);
			return {};
		}

		std::string svgLine;
		std::string svgString;
		while (!svgStream.eof())
		{
			getline(svgStream, svgLine);
			svgString += svgLine;
		}

		svgStream.close();

		removeSpaces(svgString);

		return getVerticesFromSvgString(svgString);
	}

	void SVGParser::removeSpaces(std::string& svgString)
	{
		std::size_t foundPos{};
		while ((foundPos = svgString.find(" =")) != std::string::npos)
			svgString.replace(foundPos, 2, "=");
		
		while ((foundPos = svgString.find("= ")) != std::string::npos)
			svgString.replace(foundPos, 2, "=");
		
		while ((foundPos = svgString.find(" >")) != std::string::npos)
			svgString.replace(foundPos, 2, ">");
		
		while ((foundPos = svgString.find("< ")) != std::string::npos)
			svgString.replace(foundPos, 2, "<");
	}

	std::vector<Polygon<double>> SVGParser::getVerticesFromSvgString(std::string& svgText)
	{
		std::string pathElementContent{};
		std::size_t startPosContent{};
		std::size_t endPosContent{};

		std::vector<Polygon<double>> polygons{};
		while (getElementContent(svgText, "path", pathElementContent, startPosContent, endPosContent))
		{
			Polygon<double> polygon{};

			std::string pathDataValue{};
			if (!getAttributeValue(pathElementContent, " d", pathDataValue))
			{
				std::cerr << "SVGParser::GetVerticesFromSvgString(..), path element doesn't contain a d-attribute.\n ";
				polygons.clear();
				return polygons;
			}

			if (!getVerticesFromPathData(pathDataValue, polygon))
			{
				std::cerr << "SVGParser::GetVerticesFromSvgString(..), error while extracting vertices from the path. \n";
				polygons.clear();
				return polygons;
			}

			if (polygon.vertices.empty())
			{
				std::cerr << "Empty verticesVector in GetVerticesFromSvgString(..), no vertices found in the path element" << std::endl;
				polygons.clear();
				return polygons;
			}

			polygons.push_back(polygon);
		}

		if (polygons.size() == 0)
			std::cerr << "Empty vertices in GetVerticesFromSvgString(..), no path element(s) found" << std::endl;

		return polygons;
	}

	bool SVGParser::getVerticesFromPathData(std::string_view const pathData, Polygon<double>& polygon)
	{
		std::string pathCmdChars{ "mMZzLlHhVvCcSsQqTtAa" };
		std::stringstream ss{ pathData.data() };

		char cmd{ 0 };
		Vector2<double> cursor{};
		Vector2<double> startPoint{};

		bool isOpen{ true };

		// http://www.w3.org/TR/SVG/paths.html#Introduction
		Vector2<double> vertex{};
		char pathCommand{};
		ss >> pathCommand;
		while (!ss.eof())
		{
			if (pathCmdChars.find(pathCommand) != std::string::npos)
				cmd = pathCommand;
			else
				ss.putback(pathCommand);

			switch (cmd)
			{
			case 'Z':
			case 'z':
				isOpen = true;
				break;

			case 'M':
			case 'm':
				if (isOpen)
				{
					cursor = firstSVGPoint(ss, cursor, cmd, isOpen, true);
					startPoint = cursor;
					polygon.vertices.push_back(cursor);
					isOpen = false;
					break;
				}

			case 'L':
			case 'l':
				vertex = nextSVGPoint(ss, cursor, cmd, isOpen, true);
				polygon.vertices.push_back(vertex);
				break;

			case 'h':
			case 'H':
				vertex = nextSVGCoordinateX(ss, cursor, cmd, isOpen);
				polygon.vertices.push_back(vertex);
				break;

			case 'v':
			case 'V':
				vertex = nextSVGCoordinateY(ss, cursor, cmd, isOpen);
				polygon.vertices.push_back(vertex);
				break;

			case ('C'):
			case ('c'):
				std::cerr << "SVGParser::GetVerticesFromPathData,  beziers are not supported.\nHave another look at the guide, or select all nodes in inkscape and press shift + L\n";
				return false;
				break;

			default:
				std::cerr << "SVGParser::GetVerticesFromPathData, " << cmd << " is not a supported SVG command";
				return false;
				break;
			}
			
			ss >> pathCommand;
		}

		return true;
	}

	bool SVGParser::getElementContent(std::string_view const svgText, std::string_view const elementName, std::string& elementValue, size_t& startContentPos, size_t& endContentPos)
	{
		std::size_t tempStartPos{ startContentPos };
		std::size_t tempEndPos{ endContentPos };

		std::string startElement{ std::string{ "<" } + elementName.data() + ">" };
		std::string endElement{ std::string{ "<" } + elementName.data() + "/>" };

		if ((tempStartPos = svgText.find(startElement)) != std::string::npos)
		{
			tempStartPos += startElement.length();
			if ((tempEndPos = svgText.find(endElement)) != std::string::npos)
			{
				elementValue = svgText.substr(tempStartPos, tempEndPos - tempStartPos);
				startContentPos = tempStartPos;
				endContentPos = tempEndPos;
				return true;
			}
			else
				return false;
		}

		tempStartPos = startContentPos;
		tempEndPos = endContentPos;

		startElement = std::string{ "<" } + elementName.data();
		endElement = "/>";
		if ((tempStartPos = svgText.find(startElement, tempStartPos)) != std::string::npos)
		{
			tempStartPos += startElement.length();
			if ((tempEndPos = svgText.find(endElement)) != std::string::npos)
			{
				elementValue = svgText.substr(tempStartPos, tempEndPos - tempStartPos);
				startContentPos = tempStartPos;
				endContentPos = tempEndPos;
				return true;
			}
		}

		return false;
	}

	bool SVGParser::getAttributeValue(std::string_view const svgText, std::string_view const attributeName, std::string& attributeValue)
	{
		std::string searchAttributeName{ std::string{ attributeName.data() } + "=" };

		size_t attributePos = svgText.find(searchAttributeName);
		if (attributePos == std::string::npos)
			return false;

		size_t openingDoubleQuotePos{ svgText.find("\"", attributePos) };
		if (openingDoubleQuotePos == std::string::npos)
			return false;

		size_t closingDoubleQuotePos{ svgText.find("\"", openingDoubleQuotePos + 1) };
		if (closingDoubleQuotePos == std::string::npos)
			return false;

		attributeValue = svgText.substr(openingDoubleQuotePos + 1, closingDoubleQuotePos - openingDoubleQuotePos - 1);
		return true;
	}

	void SVGParser::skipSVGComma(std::stringstream& stream, bool const isRequired)
	{
		while (true)
		{
			char c = char(stream.get());

			if (stream.eof())
			{
				if (isRequired)
					std::cerr << "SVGParser::SkipSvgComma, expected comma or whitespace\n";

				break;
			}

			if (c == (','))
				return;

			if (!isspace(c))
			{
				stream.unget();
				return;
			}
		}
	}

	double SVGParser::readSVGValue(std::stringstream& stream, double const defaultValue)
	{
		double s{};
		stream >> s;

		if (stream.eof())
			s = defaultValue;
		else
			skipSVGComma(stream, false);

		return s;
	}

	double SVGParser::readSVGValue(std::stringstream& stream, bool const separatorRequired)
	{
		float s{};
		stream >> s;
		skipSVGComma(stream, separatorRequired);
		return s;
	}

	Vector2<double> SVGParser::readSVGPoint(std::stringstream& stream)
	{
		return { readSVGValue(stream, true), readSVGValue(stream, false) };
	}

	Vector2<double> SVGParser::firstSVGPoint(std::stringstream& stream, Vector2<double>& cursor, char cmd, bool isOpen, bool advance)
	{
		if (!isOpen)
			std::cerr << "SVGParser::FirstSvgPoint, expected 'Z' or 'z' command";

		Vector2<double> p{ readSVGPoint(stream) };

		if (islower(cmd))
			p += cursor;

		if (advance)
			cursor = p;

		return p;
	}

	Vector2<double> SVGParser::nextSVGPoint(std::stringstream& stream, Vector2<double>& cursor, char cmd, bool isOpen, bool advance)
	{
		if (isOpen)
			std::cerr << "SVGParser::NextSvgPoint, expected 'M' or 'm' command\n";

		Vector2<double> p{ readSVGPoint(stream) };

		if (islower(cmd))
			p += cursor;

		if (advance)
			cursor = p;

		return p;
	}

	Vector2<double> SVGParser::nextSVGCoordinateX(std::stringstream& stream, Vector2<double>& cursor, char cmd, bool isOpen)
	{
		if (isOpen)
			std::cerr << "SVGParser::NextSvgCoordX, expected 'M' or 'm' command\n";

		double c;
		stream >> c;

		if (islower(cmd))
			cursor.x += c;
		else
			cursor.x = c;

		return cursor;
	}

	Vector2<double> SVGParser::nextSVGCoordinateY(std::stringstream& stream, Vector2<double>& cursor, char cmd, bool isOpen)
	{
		if (isOpen)
			std::cerr << "SVGParser::NextSvgCoordY, expected 'M' or 'm' command\n";

		double c{};
		stream >> c;

		if (islower(cmd))
			cursor.y += c;
		else
			cursor.y = c;

		return cursor;
	}
}