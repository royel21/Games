#ifndef _PARTICLEENGINE_H
#define _PARTICLEENGINE_H

#include <vector>

namespace Plutus {
	class ParticleBatch;
	class SpriteBatch;

	class ParticleEngine
	{
	private:
		std::vector<ParticleBatch*> m_batches;

	public:
		ParticleEngine();
		~ParticleEngine();
		void addParticleBatch(ParticleBatch* pb);
		
		void draw(SpriteBatch* sp);

		void update(float deltaTime);
	};
}


#endif