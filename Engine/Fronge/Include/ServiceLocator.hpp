#if not defined fro_SERVICE_LOCATOR_H
#define fro_SERVICE_LOCATOR_H

#include "Singleton.hpp"

#include <concepts>
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

		ServiceType& getService() const
		{
			return *m_pService.get();
		}

	private:
		ServiceLocator(ServiceLocator const&) = delete;
		ServiceLocator(ServiceLocator&&) = delete;

		ServiceLocator& operator=(ServiceLocator const&) = delete;
		ServiceLocator& operator=(ServiceLocator&&) = delete;

		std::unique_ptr<ServiceType> m_pService{ new ServiceType() };
	};
}

#endif