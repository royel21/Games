#include "ParticleBatch.h"
namespace Plutus
{

	ParticleBatch::ParticleBatch() : m_decayRate(0.01f),
									 m_maxParticles(200),
									 m_lastParticle(0),
									 m_particles(nullptr)
	{
	}

	ParticleBatch::~ParticleBatch()
	{
		delete[] m_particles;
	}

	void ParticleBatch::init(
		int maxParticles, float dr,
		GLTexture texture,
		std::function<void(Particle2D &, float)> updatefunc /* = defaultParticleUpdate*/)
	{
		m_texture = texture;
		m_maxParticles = maxParticles;
		m_particles = new Particle2D[maxParticles];
		m_decayRate = dr;
		m_updatefunc = updatefunc;
	}

	void ParticleBatch::update(float deltaTime)
	{
		for (int i = 0; i < m_maxParticles; i++)
		{
			if (m_particles[i].m_life > 0.0f)
			{
				m_updatefunc(m_particles[i], deltaTime);
				m_particles[i].m_life -= m_decayRate * deltaTime;
			}
		}
	}

	void ParticleBatch::draw(SpriteBatch *sp)
	{
		glm::vec4 uv(0.0f, 0.0f, 1.0f, 1.0f);
		for (int i = 0; i < m_maxParticles; i++)
		{
			auto &p = m_particles[i];
			if (p.m_life > 0.0f)
			{
				glm::vec4 destRect(p.m_position.x, p.m_position.y, p.m_width, p.m_width);
				sp->draw(destRect, uv, m_texture.id, 0.0f, p.m_color);
			}
		}
	}

	void ParticleBatch::addParticles(
		const glm::vec2 &position,
		const glm::vec2 &velocity,
		const ColorRGBA8 &color,
		float width)
	{
		int pIndex = getLastParticle();
		auto &p = m_particles[pIndex];

		p.m_life = 1.0f;
		p.m_color = color;
		p.m_position = position;
		p.m_velocity = velocity;
		p.m_width = width;
	}

	int ParticleBatch::getLastParticle()
	{
		int x = 0;
		for (int i = m_lastParticle; i < m_maxParticles; i++)
		{
			if (m_particles[i].m_life < 0.0f)
			{
				return i;
			}
			if (x < m_lastParticle)
			{

				if (m_particles[x].m_life < 0.0f)
				{
					return x;
				}
				x++;
			}
		}

		return 0;
	}

} // namespace Plutus