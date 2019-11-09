#include "IntRect.h"
namespace BS {
	namespace Levels {
		namespace Generator {
			int IntRect::getX(){
				return this->x;
			}
			void IntRect::setX(int value)
			{
				this->x = value;
			}
			int IntRect::getY()
			{
				return this->y;
			}
			void IntRect::setY(int value)
			{
				this->y = value;
			}
			int IntRect::getWidth()
			{
				return this->width;
			}
			void IntRect::setWidth(int value)
			{
				this->width = value;
			}
			int IntRect::getHeight()
			{
				return this->height;
			}
			void IntRect::setHeight(int value)
			{
				this->height = value;
			}
			IntRect::IntRect(int x, int y, int width, int height)
			{
				this->x = x;
				this->y = y;
				this->width = width;
				this->height = height;
			}

		}
	}
}
