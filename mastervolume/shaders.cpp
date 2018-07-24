#include "stdafx.h"
#include "shaders.h"

#include <sstream>

ShaderProgram spMain(false);

Shader::Shader() :
	shaderId(0)
{
}

Shader::Shader(const std::string &shaderPath, GLenum shaderType) :
	shaderId(glCreateShader(shaderType))
{
	// check if empty shader object creation succedded
	if (!shaderId)
		throw std::exception();

	// open file with shader source code
	std::ifstream file(shaderPath);
	if (!file.is_open())
		throw std::exception();

	// read source code
	std::string sLine, sSource;
	while (std::getline(file, sLine))
		sSource += sLine + '\n';
	const char *sourcePtr = sSource.c_str();

	// compile shader
	glShaderSource(shaderId, 1, &sourcePtr, nullptr);
	glCompileShader(shaderId);

	// check for compilation errors
	GLint result;
	glGetShaderiv(shaderId, GL_COMPILE_STATUS, &result);
	if (result != GL_TRUE) {
		// retrieve error info log and save it to file
		GLint infoLogLength;
		glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			// copy info log to buffer
			std::unique_ptr<char[]> infoLog(new char[infoLogLength]);
			glGetShaderInfoLog(shaderId, infoLogLength, nullptr, infoLog.get());

			// get name of file without an extention
			std::size_t firstLetter, extension;
			firstLetter = shaderPath.find_last_of('\\') + 1;
			extension = shaderPath.find_last_of('.');
			std::string sFileName = shaderPath.substr(firstLetter == std::string::npos ? 0 : firstLetter,
				extension == std::string::npos ? std::string::npos : extension - firstLetter);

			// open file and write log to it
			std::stringstream ss("compilation_err:");
			ss << sFileName;
			std::string sOutputFile;
			std::getline(ss, sOutputFile);

			std::ofstream compilationError(sOutputFile);
			compilationError << infoLog.get();
			compilationError.close();
		}
		throw std::exception();
	}
}

Shader::Shader(Shader &&other) :
	shaderId(other.shaderId)
{
	other.shaderId = 0;
}

Shader& Shader::operator=(Shader &&other) {
	glDeleteShader(shaderId);

	shaderId = other.shaderId;
	other.shaderId = 0;
	return *this;
}

Shader::~Shader() {
	glDeleteShader(shaderId);
}

bool Shader::create(const std::string &shaderPath, GLenum shaderType) {
	try {
		*this = Shader(shaderPath, shaderType);
		return true;
	}
	catch (std::exception&) {
		return false;
	}
}

GLenum Shader::getType() const {
	GLint type;
	glGetShaderiv(shaderId, GL_SHADER_TYPE, &type);
	return (GLenum)type;
}



ShaderProgram::ShaderProgram(bool createEmptyProgram) :
	programId(createEmptyProgram ? glCreateProgram() : 0)
{
	if (createEmptyProgram && !programId)
		throw std::exception();
}

ShaderProgram::ShaderProgram(ShaderProgram &&other) :
	programId(other.programId)
{
	other.programId = 0;
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram &&other) {
	glDeleteProgram(programId);

	programId = other.programId;
	other.programId = 0;
	return *this;
}

ShaderProgram::~ShaderProgram() {
	glDeleteProgram(programId);
}

bool ShaderProgram::create() {
	try {
		*this = ShaderProgram(true);
		return true;
	}
	catch (std::exception&) {
		return false;
	}
}

void ShaderProgram::attachShader(const Shader &shaderObj) {
	glAttachShader(programId, shaderObj.getId());
}

void ShaderProgram::detachShader(const Shader &shaderObj) {
	glDetachShader(programId, shaderObj.getId());
}

bool ShaderProgram::linkProgram() {
	if (!programId)
		return false;

	glLinkProgram(programId);
	GLint result;
	glGetProgramiv(programId, GL_LINK_STATUS, &result);
	if (result != GL_TRUE) {
		GLint infoLogLength;
		glGetProgramiv(programId, GL_INFO_LOG_LENGTH, &infoLogLength);
		if (infoLogLength > 0) {
			std::unique_ptr<char[]> infoLog(new (std::nothrow) char[infoLogLength]);
			if (infoLog) {
				glGetProgramInfoLog(programId, infoLogLength, nullptr, infoLog.get());

				std::ofstream outputFile("linking_error");
				outputFile << infoLog.get();
				outputFile.close();
			}
		}
		return false;
	}
	return true;
}

void ShaderProgram::useProgram() {
	glUseProgram(programId);
}

void ShaderProgram::setUniform(const char *uniformName, const glm::mat4 &value, bool transpose) {
	GLuint uniformLocation = glGetUniformLocation(programId, uniformName);
	glUniformMatrix4fv(uniformLocation, 1, transpose ? GL_TRUE : GL_FALSE, &value[0][0]);
}

void ShaderProgram::setUniform(GLuint uniformLocation, const glm::mat4 &value, bool transpose) {
	glUniformMatrix4fv(uniformLocation, 1, transpose ? GL_TRUE : GL_FALSE, &value[0][0]);
}

void ShaderProgram::setUniform(const char *uniformName, const glm::vec3 &value) {
	GLuint uniformLocation = glGetUniformLocation(programId, uniformName);
	glUniform3fv(uniformLocation, 1, &value[0]);
}

void ShaderProgram::setUniform(GLuint uniformLocation, const glm::vec3 &value) {
	glUniform3fv(uniformLocation, 1, &value[0]);
}
