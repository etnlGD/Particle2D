#pragma once

namespace particle2d
{
	template <typename T>
	class IDistribution
	{
	public:
		virtual ~IDistribution() {}

		virtual T generate() = 0;

		template <typename URNG, typename StdDistribution>
		static IDistribution<T>* create(const StdDistribution& distribution)
		{
			class DistributionTpl : public IDistribution<T>
			{
			public:
				DistributionTpl(const StdDistribution& copy, unsigned seed) : 
					impl(copy), generator(++seed) {}

				virtual T generate() override
				{
					return impl(generator);
				}

			private:
				StdDistribution impl;
				URNG generator;
			};


			static unsigned seed = (unsigned) (&distribution);
			return new DistributionTpl(distribution, ++seed);
		}
	};
}

