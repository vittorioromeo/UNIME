// Copyright (c) 2013-2014 Vittorio Romeo
// License: Academic Free License ("AFL") v. 3.0
// AFL License page: http://opensource.org/licenses/AFL-3.0

#ifndef SSVOB_PARTICLES_PARTICLESYSTEM
#define SSVOB_PARTICLES_PARTICLESYSTEM

#include "SSVBloodshed/Particles/OBParticle.hpp"
#include "SSVBloodshed/OBCommon.hpp"
#include "SSVBloodshed/OBConfig.hpp"

namespace ob
{
namespace test{

using Idx = std::size_t;
using Ctr = int;

template<typename> class Manager;
template<typename> class Handle;

enum class State : int { Alive = 0, Unused = 1, Dead = 2  };

template<typename T> class Atom
{
	SSVU_ASSERT_STATIC(ssvu::isDefaultConstructible<T>(), "T must be default-constructible");

	template<typename> friend class Manager;
	template<typename> friend class Handle;

	public:
		Idx ctrlIdx;
		State state{State::Unused};
		T impl;

		template<typename... TArgs> inline void initImpl(TArgs&&... mArgs)
			noexcept(noexcept(T(std::forward<TArgs>(mArgs)...)))
		{
			impl = T(std::forward<TArgs>(mArgs)...);
		}
};

class Controller
{
	template<typename> friend class Manager;
	template<typename> friend class Handle;

	private:
		Idx idx;
		Ctr ctr;
};

template<typename T> class Handle
{
	template<typename> friend class Manager;

	private:
		Manager<T>& manager;
		Idx ctrlIdx;
		Ctr ctr;

		inline Handle(Manager<T>& mManager, Idx mCtrlIdx, Ctr mCtr) noexcept
			: manager(mManager), ctrlIdx{mCtrlIdx}, ctr{mCtr} { }

		Atom<T>& getAtom() noexcept;

	public:
		T& get() noexcept;
		bool isAlive() noexcept;
		void destroy() noexcept;
};

template<typename T> class Manager
{
	template<typename> friend class Handle;

	public:
		std::vector<Atom<T>> atoms;
		std::vector<Controller> controllers;
		Idx lastFree{0u};

		inline void resizeStorage(std::size_t mOldSize, std::size_t mNewSize)
		{
			SSVU_ASSERT(mNewSize >= 0);

			atoms.resize(mNewSize);
			controllers.resize(mNewSize);

			// Initialize resized storage
			for(; mOldSize < mNewSize; ++mOldSize)
			{
				atoms[mOldSize].ctrlIdx = mOldSize;
				controllers[mOldSize].idx = mOldSize;
			}
		}

		inline void resizeIfNeeded()
		{
			constexpr std::size_t resizeAmount{10};

			// If the last free index is valid, return
			auto oldSize(atoms.size());
			if(oldSize > lastFree) return;

			// Calculate new size and resize storage
			resizeStorage(oldSize, oldSize + resizeAmount);
		}

		inline void destroy(Idx mCtrlIdx) noexcept
		{
			atoms[controllers[mCtrlIdx].idx].state = State::Dead;
		}

	public:
		inline Manager() { }

		inline void clear() noexcept
		{
			atoms.clear();
			controllers.clear();
			lastFree = 0;
		}

		inline void reserve(std::size_t mSize)
		{
			resizeStorage(atoms.size(), mSize);
		}

		template<typename... TArgs> inline Handle<T> createAtom(TArgs&&... mArgs)
		{
			resizeIfNeeded();

			atoms[lastFree].initImpl(std::forward<TArgs>(mArgs)...);
			atoms[lastFree].state = State::Alive;

			auto cIdx(atoms[lastFree].ctrlIdx);
			auto& controller(controllers[cIdx]);
			controller.idx = lastFree;
			++controller.ctr;
			++lastFree;

			return {*this, cIdx, controller.ctr};
		}

		inline void refresh()
		{
			// C++14: use polymorphic lambda
			ssvu::sortStable(atoms, [](const Atom<T>& mA, const Atom<T>& mB){ return (int)mA.state < (int)mB.state; });

			auto rIdx(atoms.size() - 1);
			for(; atoms[rIdx].state == State::Dead; --rIdx)
			{
				atoms[rIdx].state = State::Unused;
				auto& controller(controllers[atoms[rIdx].ctrlIdx]);
				++controller.ctr;
				//controller.idx = -1;
			}
			auto fIdx(0u);
			for(; atoms[fIdx].state == State::Alive; ++fIdx) controllers[atoms[fIdx].ctrlIdx].idx = fIdx;
			lastFree = fIdx;
		}

		template<typename TFunc> inline void forEach(TFunc mFunc)
		{
			for(auto i(0u); i < lastFree; ++i) mFunc(atoms[i].impl);
		}
		template<typename TFunc> inline void forEachAtom(TFunc mFunc)
		{
			for(auto i(0u); i < lastFree; ++i) mFunc(atoms[i]);
		}
		inline void destroyAt(Idx mCtrlIdx) noexcept
		{
			atoms[mCtrlIdx].state = State::Dead;
		}

		inline std::size_t getSize() { return lastFree; }
};

template<typename T> inline Atom<T>& Handle<T>::getAtom() noexcept
{
	SSVU_ASSERT(isAlive());
	return manager.atoms[manager.controllers[ctrlIdx].idx];
}

template<typename T> inline T& Handle<T>::get() noexcept
{
	return getAtom().impl;
}

template<typename T> inline bool Handle<T>::isAlive() noexcept
{
	return manager.controllers[ctrlIdx].ctr == ctr;
}

template<typename T> inline void Handle<T>::destroy() noexcept
{
	return manager.destroy(ctrlIdx);
}
}

	class OBParticleSystem : public sf::Drawable
	{
		private:
			ssvs::VertexVector<sf::PrimitiveType::Quads> vertices;
			test::Manager<OBParticle> particles;
			std::size_t currentCount{0};

		public:
			inline OBParticleSystem() { vertices.resize(OBConfig::getParticleMax() * 4); particles.reserve(OBConfig::getParticleMax()); }
			template<typename... TArgs> inline void emplace(TArgs&&... mArgs) { particles.createAtom(std::forward<TArgs>(mArgs)...); }
			inline void update(FT mFT)
			{

				particles.refresh();

				for(auto i(0u); i < particles.atoms.size(); ++i)
				{
						auto& p(particles.atoms[i]);
						 if(p.impl.life <= 0) p.state = test::State::Dead;
				}

				currentCount = particles.getSize();


				for(auto i(0u); i < currentCount; ++i)
				{
					auto& p(particles.atoms[i].impl);
					p.update(mFT);
					const auto& vIdx(i * 4);

					auto& vNW(vertices[vIdx + 0]);
					auto& vNE(vertices[vIdx + 1]);
					auto& vSE(vertices[vIdx + 2]);
					auto& vSW(vertices[vIdx + 3]);

					vNW.position = p.nw;
					vNE.position = p.ne;
					vSE.position = p.se;
					vSW.position = p.sw;

					vNW.color = vNE.color = vSE.color = vSW.color = p.color;

				}
			}
			inline void draw(sf::RenderTarget& mRenderTarget, sf::RenderStates mRenderStates) const override { mRenderTarget.draw(&vertices[0], currentCount * 4, sf::PrimitiveType::Quads, mRenderStates); }
			inline void clear() noexcept { particles.clear(); currentCount = 0; }
	};
}

#endif
