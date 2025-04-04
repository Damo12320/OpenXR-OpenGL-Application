#include "GBuffer.h"

GBuffer::GBuffer(float width, float height) {
	framebuffer = std::make_unique<Framebuffer>();

	this->CreatePositionTexture(width, height);
	this->CreateNormalTexture(width, height);
	this->CreateAlbedoSpecTexture(width, height);
	this->CreateDepthTexture(width, height);

	glBindFramebuffer(GL_FRAMEBUFFER, this->framebuffer->GetID());

	unsigned int attachments[3] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2 };
	glNamedFramebufferDrawBuffers(this->framebuffer->GetID(), 3, attachments);

	if (!framebuffer->IsComplete()) {
		std::cout << "GBUFFER::Framebuffer not complete" << std::endl;
	}

	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

GBuffer::~GBuffer() {
	std::cout << "GBUFFER::Deleting" << std::endl;
}

void GBuffer::Bind() {
	this->framebuffer->Bind();

	////Check if the draw buffers are set correctly (Debugging)
	//for (int i = 0; i < 3; ++i) {
	//	GLint drawBuffer;
	//	glGetIntegerv(GL_DRAW_BUFFER0 + i, &drawBuffer);
	//	std::cout << "Draw Buffer " << i << ": " << drawBuffer << std::endl;
	//}
}

void GBuffer::Clear() {
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

GLTexture2D* GBuffer::GetPositionTexture() {
	return framebuffer->GetAttachedTexture(GL_COLOR_ATTACHMENT0);
}

GLTexture2D* GBuffer::GetNormalTexture() {
	return framebuffer->GetAttachedTexture(GL_COLOR_ATTACHMENT1);
}

GLTexture2D* GBuffer::GetAlbedoSpecTexture() {
	return framebuffer->GetAttachedTexture(GL_COLOR_ATTACHMENT2);
}

GLTexture2D* GBuffer::GetDepthTexture() {
	return framebuffer->GetAttachedTexture(GL_DEPTH_ATTACHMENT);
}



//private functions
void GBuffer::CreatePositionTexture(float width, float height) {
	this->framebuffer->AttachNewTexture2D(GL_COLOR_ATTACHMENT0, width, height, GL_RGBA16F);
}

void GBuffer::CreateNormalTexture(float width, float height) {
	this->framebuffer->AttachNewTexture2D(GL_COLOR_ATTACHMENT1, width, height, GL_RGBA16F);
}

void GBuffer::CreateAlbedoSpecTexture(float width, float height) {
	this->framebuffer->AttachNewTexture2D(GL_COLOR_ATTACHMENT2, width, height, GL_RGBA16F);
}

void GBuffer::CreateDepthTexture(float width, float height) {
	this->framebuffer->AttachNewTexture2D(GL_DEPTH_ATTACHMENT, width, height, GL_DEPTH_COMPONENT24);
}