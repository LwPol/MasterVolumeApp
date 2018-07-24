#pragma once

#include "stdafx.h"
#include <vector>

class Shader {
	private:
		GLuint shaderId;
	public:
		Shader();
		Shader(const std::string &shaderPath, GLenum shaderType);
		Shader(const Shader&) = delete;
		Shader(Shader &&other);

		Shader& operator=(const Shader&) = delete;
		Shader& operator=(Shader &&other);

		~Shader();

		bool create(const std::string &shaderPath, GLenum shaderType);

		GLenum getType() const;

		operator bool() const { return shaderId != 0; }

		GLuint getId() const { return shaderId; }
};

class ShaderProgram {
	private:
		GLuint programId;
	public:
		ShaderProgram(bool createEmptyProgram = true);
		ShaderProgram(const ShaderProgram&) = delete;
		ShaderProgram(ShaderProgram &&other);
		template <class Container>
		ShaderProgram(const Container &shaders);

		ShaderProgram& operator=(const ShaderProgram&) = delete;
		ShaderProgram& operator=(ShaderProgram &&other);

		~ShaderProgram();

		bool create();

		void attachShader(const Shader &shaderObj);
		void detachShader(const Shader &shaderObj);

		bool linkProgram();

		void useProgram();

		void setUniform(const char *uniformName, const glm::mat4 &value, bool transpose = false);
		void setUniform(GLuint uniformLocation, const glm::mat4 &value, bool transpose = false);
		void setUniform(const char *uniformName, const glm::vec3 &value);
		void setUniform(GLuint uniformLocation, const glm::vec3 &value);

		operator bool() const { return programId != 0; }

		GLuint getId() const { return programId; }
};

template <class Container>
ShaderProgram::ShaderProgram(const Container &shaders) :
	programId(glCreateProgram())
{
	static_assert(std::is_same<Shader, Container::value_type>::value,
		"Container must store Shader objects");

	if (!programId)
		throw std::exception();

	for (auto &&sh : shaders) {
		if (sh)
			attachShader(sh);
	}

	if (!linkProgram())
		throw std::exception();
}

extern ShaderProgram spMain;