#include "ParticleEngine.h"
#include "ParticleBatch.h"
#include "Graphics/SpriteBatch.h"

namespace Plutus
{
	ParticleEngine::ParticleEngine()
	{
	}

	ParticleEngine::~ParticleEngine()
	{
		for (auto &b : m_batches)
		{
			delete b;
		}
	}
	void ParticleEngine::addParticleBatch(ParticleBatch *pb)
	{
		m_batches.push_back(pb);
	}
	void ParticleEngine::draw(SpriteBatch *sp)
	{
		for (auto &b : m_batches)
		{
			sp->begin();
			b->draw(sp);
			sp->end();
		}
	}
	void ParticleEngine::update(float deltaTime)
	{
		for (auto &b : m_batches)
		{
			b->update(deltaTime);
		}
	}
} // namespace Plutus