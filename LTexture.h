#include <string>
#include <SDL_image.h>
#include <SDL.h>
#include <SDL_ttf.h>

class LTexture
{
public:
	//Initializes variables
	LTexture();

	//Deallocates memory
	~LTexture();

	//Loads image at specified path
	bool loadFromFile(std::string path, SDL_Renderer* gRenderer);

	//Deallocates texture
	void free();

	//Creates image from font string
	bool loadFromRenderedText(std::string textureText, SDL_Color textColor, SDL_Renderer* gRenderer, TTF_Font* gFont);

	//Renders texture at given point
	void render(int x, int y, SDL_Renderer* gRenderer);

	void setBlendMode(SDL_BlendMode blending);

	void setAlpha(Uint8 alpha);

	//Gets image dimensions
	int getWidth();

	int getHeight();

private:
	//The actual hardware texture
	SDL_Texture* mTexture;

	//Image dimensions
	int mWidth;
	int mHeight;
};
