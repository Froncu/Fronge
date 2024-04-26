#pragma once

namespace fro
{
	class Command
	{
	public:
		virtual ~Command() = default;

		virtual void operator()() = 0;

	protected:
		Command() = default;

	private:
		Command(Command const&) = delete;
		Command(Command&&) noexcept = delete;

		Command& operator=(Command const&) = delete;
		Command& operator=(Command&&) noexcept = delete;
	};
}