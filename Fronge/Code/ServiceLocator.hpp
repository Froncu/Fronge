#pragma once

#include "Singleton.hpp"

#include <memory>

namespace fro
{
	template<typename ServiceType>
	class ServiceLocator final : public Singleton<ServiceLocator<ServiceType>>
	{
	public:
		ServiceLocator() = default;

		virtual ~ServiceLocator() override = default;

		template<typename ProviderType>
		requires std::derived_from<ProviderType, ServiceType>
		void setProvider()
		{
			m_pService.reset(new ProviderType());
		}

		ServiceType& getService()
		{
			return *m_pService.get();
		}

	private:
		ServiceLocator(const ServiceLocator&) = delete;
		ServiceLocator(ServiceLocator&&) = delete;

		ServiceLocator& operator=(const ServiceLocator&) = delete;
		ServiceLocator& operator=(ServiceLocator&&) = delete;

		std::unique_ptr<ServiceType> m_pService{ new ServiceType() };
	};
}