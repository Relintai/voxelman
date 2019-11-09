#pragma once
#include <System/System.h>

using namespace System;
namespace BS {
	namespace Levels {
		namespace Generator {
			class IntRect : public virtual Object
			{
				private:
					int x;
				private:
					int y;
				private:
					int width;
				private:
					int height;
				public:
					int getX();
				public:
					void setX(int value);
				public:
					int getY();
				public:
					void setY(int value);
				public:
					int getWidth();
				public:
					void setWidth(int value);
				public:
					int getHeight();
				public:
					void setHeight(int value);
				public:
					IntRect(int x, int y, int width, int height);
			};
		}
	}
}
