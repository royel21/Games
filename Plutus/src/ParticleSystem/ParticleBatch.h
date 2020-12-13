#ifndef _PARTICLEBATCH_H
#define _PARTICLEBATCH_H

#include <glm/glm.hpp>
#include <functional>

#include "Graphics/vertex.h"
#include "Graphics/SpriteBatch.h"
#include "Texture/Texture.h"

namespace Plutus
{

	struct Particle2D
	{
		glm::vec2 m_position;
		glm::vec2 m_velocity;
		float m_width;
		ColorRGBA8 m_color;
		float m_life;
	};

	inline void defaultParticleUpdate(Particle2D &p, float deltaTime)
	{
		p.m_position += p.m_velocity * deltaTime;
	}

	class ParticleBatch
	{
	private:
		float m_decayRate;
		int m_maxParticles;
		int m_lastParticle;
		GLTexture m_texture;
		Particle2D *m_particles;
		std::function<void(Particle2D &, float)> m_updatefunc;

	public:
		ParticleBatch();
		~ParticleBatch();

		void init(
			int maxParticles,
			float dr,
			GLTexture tx,
			std::function<void(Particle2D &, float)> updatefunc = defaultParticleUpdate);

		void update(float deltaTime);

		void draw(SpriteBatch *sp);

		void addParticles(
			const glm::vec2 &position,
			const glm::vec2 &velocity,
			const ColorRGBA8 &color,
			float width);

	private:
		int getLastParticle();
	};

} // namespace Plutus

#endif
