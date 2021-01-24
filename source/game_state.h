#pragma once

#include <SFML/Graphics.hpp>

class tiny_state
{
public:
	virtual void Initialize(sf::RenderWindow* window)
	{
	}

	virtual void Update(sf::RenderWindow* window)
	{
	}

	virtual void Render(sf::RenderWindow* window)
	{
	}

	virtual void Destroy(sf::RenderWindow* window)
	{
	}
};

class game_state
{
public:
	game_state()
	{
		this->state = NULL;
		this->window = NULL;
		this->event = NULL;
	}

	void SetWindow(sf::RenderWindow* window)
	{
		if (this->window == NULL)
			this->window = window;
		else
		{
			//this->window->close();
			this->window = window;
		}
	}

	sf::RenderWindow* GetWindow()
	{
		if (this->window != NULL)
			return this->window;
		else
		{
			this->SetWindow(new sf::RenderWindow());
			return this->window;
		}
	}

	void SetEvent(sf::Event* event)
	{
		this->event = event;
	}

	sf::Event* GetEvent()
	{
		if (this->event != NULL)
			return this->event;
		else
		{
			this->SetEvent(new sf::Event());
			return this->event;
		}
	}

	void SetState(tiny_state* state)
	{
		if (this->state != NULL)
		{
			this->state->Destroy(this->window);
		}
		this->state = state;
		if (this->state != NULL)
		{
			this->state->Initialize(this->window);
		}
	}

	void Update()
	{
		if (this->state != NULL)
		{
			this->state->Update(this->window);
		}
	}
	void Render()
	{
		if (this->state != NULL)
		{
			this->state->Render(this->window);
		}
	}

	~game_state()
	{
		if (this->state != NULL)
		{
			this->state->Destroy(this->window);
		}
	}
private:
	sf::RenderWindow* window;
	sf::Event* event;
	tiny_state* state;
};

extern game_state coreState;
extern sf::Color bg_color;
extern bool quitGame;
extern std::string login;
extern bool loading;

