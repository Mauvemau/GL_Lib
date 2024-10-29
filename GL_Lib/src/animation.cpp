#include "animation.h"

#include "lib_time.h"
#include "transform.h"

namespace gllib {

	animation::animation(Vector3 position, Vector3 scale, Color color): sprite(position, {0, 0, 0}, scale, color)
	{
		currentFrameIndex = 0;

		elapsedTime = 0.00;
		durationInSecs = 0.00;

		mirrorX = false;
		mirrorY = false;
	}

	animation::~animation() {
	}
	
	void animation::update() {
		elapsedTime += LibTime::getDeltaTime();

		double normalizedTime = fmod(elapsedTime, durationInSecs);
		int totalFrames = static_cast<int>(sprite.getTextures().size());

		if (totalFrames < 1)
			return;

		int framesPerSecond = static_cast<int>(totalFrames / durationInSecs);
		int currentFrame = static_cast<int>(normalizedTime * framesPerSecond) % totalFrames;

		if (mirrorX) {
			currentFrameIndex = totalFrames - 1 - currentFrame;
		}
		else {
			currentFrameIndex = currentFrame;
		}
	}

	void animation::addFrame(unsigned int textureID, float offsetX, float offsetY, float width, float height, int frameAmount) {
		Frame frame;
		frame.textureID = textureID;

		for (int i = 0; i < frameAmount; i++)
		{
			
			frame.uvCoords[0] = { offsetX + width, offsetY + height };
			frame.uvCoords[1] = { offsetX + width, offsetY };
			frame.uvCoords[2] = { offsetX, offsetY };
			frame.uvCoords[3] = { offsetX, offsetY + height };
	
			sprite.addFrame(frame);
			offsetX += width;
			offsetY += height;
		}
	}
	

	void animation::setSpriteSheet(unsigned int spriteSheetID, int amountColumns, int amountRows, double durationInSecs) {

		float frameWidth = 1.0f / amountColumns;
		float frameHeight = 1.0f / amountRows;
		int amountFrames = amountColumns * amountRows;
		this->durationInSecs = durationInSecs;

		for (int row = 0; row < amountRows; ++row) {
			for (int col = 0; col < amountColumns; ++col) {
				float offsetX = col * frameWidth;
				float offsetY = row * frameHeight;
				addFrame(spriteSheetID, offsetX, offsetY, frameWidth, frameHeight, 1);
			}
		}
	}

	void animation::setDurationInSecs(double durationInSecs) {
		this->durationInSecs = durationInSecs;
	}

	void animation::setCurrentFrame(int frame) {
		if (frame > 0 && frame < sprite.getTextures().size()) {
			currentFrameIndex = frame;
		}
		else {
			std::cout << "Frame out of bounds!\n";
		}
	}

	void animation::setMirrorX(bool mirrorX) {
		if (this->mirrorX == mirrorX) return;
		this->mirrorX = mirrorX;
		for (auto& anim : sprite.getTextures()) {
			for (auto& frame : anim) {
				for (int i = 0; i < 4; ++i) {
					frame.uvCoords[i].u = 1.0f - frame.uvCoords[i].u;
				}
			}
		}
	}

	void animation::setMirrorY(bool mirrorY) {
		if (this->mirrorY == mirrorY) return;
		this->mirrorY = mirrorY;
		for (auto& anim : sprite.getTextures()) {
			for (auto& frame : anim) {
				for (int i = 0; i < 4; ++i) {
					frame.uvCoords[i].v = 1.0f - frame.uvCoords[i].v;
				}
			}
		}
	}

	void animation::draw() {
		sprite.draw(); 
	}

}