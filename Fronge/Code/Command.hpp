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
		Command(const Command&) = delete;
		Command(Command&&) noexcept = delete;

		Command& operator=(const Command&) = delete;
		Command& operator=(Command&&) noexcept = delete;
	};
}