#ifndef SVG_PARSER_HPP
#define SVG_PARSER_HPP

// NOTE: yoinked from DAE's game engine for Programming 2 and modified a bit

#include "froch.hpp"

#include "Core.hpp"
#include "Maths/Structs/Polygon.hpp"

namespace fro
{
	class SVGParser final
	{
	public:
		FRO_API FRO_NODISCARD static std::vector<Polygon<double>> parse(std::string_view const filePath);

	private:
		static void removeSpaces(std::string& svgString);
		FRO_NODISCARD static std::vector<Polygon<double>> getVerticesFromSvgString(std::string& svgText);
		FRO_NODISCARD static bool getVerticesFromPathData(std::string_view const pathData, Polygon<double>& polygon);
		FRO_NODISCARD static bool getElementContent(std::string_view const svgText, std::string_view const elementName, std::string& elementValue, size_t& startContentPos, size_t& endContentPos);
		FRO_NODISCARD static bool getAttributeValue(std::string_view const svgText, std::string_view const attributeName, std::string& attributeValue);

		FRO_NODISCARD static void skipSVGComma(std::stringstream& stream, bool const isRequired);
		FRO_NODISCARD static double readSVGValue(std::stringstream& stream, double const defaultValue);
		FRO_NODISCARD static double readSVGValue(std::stringstream& stream, bool const separatorRequired);

		FRO_NODISCARD static Vector2<double> readSVGPoint(std::stringstream& stream);

		FRO_NODISCARD static Vector2<double> firstSVGPoint(std::stringstream& stream, Vector2<double>& cursor, char cmd, bool isOpen, bool advance);

		FRO_NODISCARD static Vector2<double> nextSVGPoint(std::stringstream& stream, Vector2<double>& cursor, char cmd, bool isOpen, bool advance);

		FRO_NODISCARD static Vector2<double> nextSVGCoordinateX(std::stringstream& stream, Vector2<double>& cursor, char cmd, bool isOpen);

		FRO_NODISCARD static Vector2<double> nextSVGCoordinateY(std::stringstream& stream, Vector2<double>& cursor, char cmd, bool isOpen);
	};
}

#endif