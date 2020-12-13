#ifndef _SHADER_H
#define _SHADER_H

#include <iostream>
#include "GLheaders.h"
#include <unordered_map>
#include <string>
#include <glm/glm.hpp>

namespace Plutus
{

	class Shader
	{
	private:
		uint32_t m_progId;
		int m_numAttributes;
		std::unordered_map<std::string, GLint> uniformLocationsMap;
		bool compileError = false;

	public:
		Shader();
		~Shader();

		bool CreateProgWithShader(std::string vsPath, std::string fsPath);

		void enable();
		void disable();
		void dispose();

		uint32_t getUniform(std::string uName);

		uint32_t getProg() { return m_progId; }

		void setAtribute(const std::string &attributeName);

		void setUniform1i(std::string name, GLuint value);

		void setUniform1f(std::string name, float value);

		void setUniform1iv(std::string name, GLint *value, GLuint size);

		void setUniform2f(std::string name, glm::vec2 value);

		void setUniform3f(std::string name, glm::vec3 value);

		void setUniform4f(std::string name, const glm::vec4 &value);

		void setUniformMat4(std::string name, const glm::mat4 &value);

	private:
		void compileShader(uint32_t id, std::string shader);
		bool getCompileError(uint32_t id, std::string shader);
		void cleanUp(GLuint verId, GLuint fragId);
	};

} // namespace Plutus

#endif