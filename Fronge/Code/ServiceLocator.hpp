#pragma once

#include "Singleton.hpp"
#include "AudioSDL.h"

#include <memory>

namespace fro
{
	// TODO: avoided using the singleton generated defines since it cause problems; do something about it
	template<typename ServiceType>
	class ServiceLocator final : public Singleton<ServiceLocator<ServiceType>>
	{
		friend class Singleton<ServiceLocator<ServiceType>>;

	public:
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
		ServiceLocator() = default;
		ServiceLocator(const ServiceLocator&) = delete;
		ServiceLocator(ServiceLocator&&) = delete;

		ServiceLocator& operator=(const ServiceLocator&) = delete;
		ServiceLocator& operator=(ServiceLocator&&) = delete;

		std::unique_ptr<ServiceType> m_pService{ new ServiceType() };
	};
	// END TODO
}