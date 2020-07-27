#include "include/Game.h"


#define BACKGROUND "image/background.png"
#define MUSIC "music/music.wav"
#define FONT "font/arial.ttf"

#define MEDIKIT_VALUE 50
#define ASTEROID_DAMAGE 25

Uint8 *audio_pos; // global pointer to the audio buffer to be played
Uint32 audio_len; // remaining length of the sample we have to play

void warning_off(void *arg) {
	(void) arg;
}

void my_audio_callback(void *userdata, Uint8 *stream, int len) {

	len = ( (unsigned int) len > audio_len ? audio_len : len );
	SDL_memcpy (stream, audio_pos, len);
	SDL_MixAudio(stream, audio_pos, len, SDL_MIX_MAXVOLUME);

	audio_pos += len;
	audio_len -= len;

	warning_off(userdata);
}

float distance(Point p1, Point p2) {
	float x = powf(p1.getX()-p2.getX(),2);
	float y = powf(p1.getY()-p2.getY(),2);

	return sqrtf(x+y);
}

void Game::playSong(){
	SDL_CloseAudio();

	if( SDL_LoadWAV(s.name, &s.wav_spec, &s.wav_buffer, &s.wav_length) == NULL ){
		return ;
	}

	// set the callback function
	s.wav_spec.callback = my_audio_callback;
	s.wav_spec.userdata = NULL;
	// set our global static variables
	s.audio_pos = s.wav_buffer; // copy sound buffer
	s.audio_len = s.wav_length; // copy file length

	/* Open the audio device */
	if ( SDL_OpenAudio(&s.wav_spec, NULL) < 0 ){
		fprintf(stderr, "Couldn't open audio: %s\n", SDL_GetError());
		exit(-1);
	}

	audio_pos = s.audio_pos;
	audio_len = s.audio_len;


	SDL_PauseAudio(0);
}

Game::Game() {

  if (SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO) != 0)
	{
		return;
	}


	window = SDL_CreateWindow("Asteroids", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, WIDTH, HEIGHT,
						  SDL_WINDOW_SHOWN|SDL_WINDOW_ALLOW_HIGHDPI);
	renderer = SDL_CreateRenderer(window, -1,
			SDL_RENDERER_ACCELERATED|SDL_RENDERER_PRESENTVSYNC);

	SDL_GetRendererOutputSize(renderer, &width, &height);
	SDL_SetRenderDrawBlendMode(renderer,SDL_BLENDMODE_ADD);

	// TEXT
	if(TTF_Init()==-1) {
    printf("TTF_Init: %s\n", TTF_GetError());
    return;
	}

	font = TTF_OpenFont(FONT, 100); //this opens a font style and sets a size
	if(!font) {
			printf("TTF_OpenFont: %s\n", TTF_GetError());
			// handle error
	}

	// SOUND
	s.name = MUSIC;
	playSong();


	// BACKGROUND
	surface = IMG_Load(BACKGROUND);
	if(!surface)
	{
	    printf("Erreur de chargement de l'image : %s",SDL_GetError());
	    return ;
	}
	background = SDL_CreateTextureFromSurface(renderer,surface);  //La texture monImage contient maintenant l'image importée

	//RANDOM
	srand(time(NULL));
}

Game::~Game() {
	SDL_CloseAudio();

	if(s.wav_buffer != nullptr)
		SDL_FreeWAV(s.wav_buffer);
	SDL_FreeSurface(surface);
	SDL_DestroyTexture(texture);
	SDL_DestroyTexture(background);
  SDL_DestroyRenderer(renderer);
  SDL_DestroyWindow(window);
	TTF_Quit();
  SDL_Quit();

}

void Game::new_shot(Point p, float angle) {
  Shot s(p, angle);
  s_list.push_back(s);
}

void Game::new_asteroid(int size, int nb, Point p) {
	for (int i(0) ; i < nb ; i++) {
		Asteroid a(size, p);
		a_list.push_back(a);
	}
};

void Game::new_asteroid(int size, int nb){
	for (int i(0) ; i < nb ; i++) {
		Asteroid a(size);
		a_list.push_back(a);
	}
};

void Game::updateScore(int life) {
		SDL_Color White = {255, 255, 255, 255};
		char buf[256];

		sprintf(buf, "%u", score);
		float tmp = log10(score);
		if(tmp < 0) tmp = 1;
		tmp = floor(tmp);
		surface = TTF_RenderText_Solid(font, buf , White);
		texture = SDL_CreateTextureFromSurface(renderer, surface);

		SDL_Rect Message_rect;

		Message_rect.x = 15;
		Message_rect.y = 15;
		Message_rect.w = 30*tmp;
		Message_rect.h = 25;

		SDL_RenderCopy(renderer, texture, NULL, &Message_rect);

		// Life
		SDL_Rect rect;
		rect.x = 15;
		rect.y = 50;
		rect.w = life;
		rect.h = 20;
		SDL_SetRenderDrawColor(renderer, 0, 255, 0, 150);
		SDL_RenderFillRect( renderer, &rect );
		rect.w = 100 - life;
		rect.x = 15 + life;
		SDL_SetRenderDrawColor(renderer, 255, 0, 0, 150);
		SDL_RenderFillRect( renderer, &rect );

		SDL_SetRenderDrawColor(renderer,255, 255, 255, 255);
		rect.x = 15;
		rect.w = 100;
		SDL_RenderDrawRect(renderer, &rect);
}

void Game::menu(){
  bool start = false;
  while (!start && !end_b)
	{
		SDL_Event event;
		while (!start && SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				end_b = true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					// touche clavier
					case SDLK_RETURN:
						start = true;
						break;

					case SDLK_ESCAPE:
            end_b = true;
            break;
					default:
            break;
				}
				break;

			default: break;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0,0,0,255);
		SDL_RenderClear(renderer);

		SDL_RenderCopy(renderer, background, NULL, NULL);


		// TEXT
		SDL_Color White = {255, 255, 255, 255};

		SDL_Rect Message_rect;

		// ASTEROID
		surface = TTF_RenderText_Solid(font, "ASTEROIDS" , White);
		texture = SDL_CreateTextureFromSurface(renderer, surface);

		Message_rect.w = 500;
		Message_rect.h = 150;
		Message_rect.x = (WIDTH-Message_rect.w)*0.5;
		Message_rect.y = (HEIGHT-Message_rect.h)*0.2;

		SDL_RenderCopy(renderer, texture, NULL, &Message_rect);

		// PRESS ENTER
		surface = TTF_RenderText_Solid(font, "PRESS ENTER TO PLAY" , White);
		texture = SDL_CreateTextureFromSurface(renderer, surface);

		Message_rect.w = 400;
		Message_rect.h = 75;
		Message_rect.x = (WIDTH-Message_rect.w)*0.5;
		Message_rect.y = (HEIGHT-Message_rect.h)*0.5;

		if (fmod(SDL_GetTicks(),500) > 50) {
			SDL_RenderCopy(renderer, texture, NULL, &Message_rect);
		}

		// PRESS ENTER
		surface = TTF_RenderText_Solid(font, "BY ANAS & BAPTISTE" , White);
		texture = SDL_CreateTextureFromSurface(renderer, surface);

		Message_rect.w = 250;
		Message_rect.h = 25;
		Message_rect.x = (WIDTH-Message_rect.w)*0.5;
		Message_rect.y = (HEIGHT-Message_rect.h)*0.9;

		SDL_RenderCopy(renderer, texture, NULL, &Message_rect);


		SDL_RenderPresent(renderer);

		// SOUND
		if(audio_len == 0) {
			audio_pos = s.audio_pos;
			audio_len = s.audio_len;
		}

    SDL_Delay(DELAY);
  }
};

void Game::end(){
  bool menu_b = false;
  while (!menu_b && !end_b)
	{
		SDL_Event event;
		while (!menu_b && SDL_PollEvent(&event))
		{
			switch (event.type)
			{
			case SDL_QUIT:
				end_b = true;
				break;
			case SDL_KEYDOWN:
				switch (event.key.keysym.sym)
				{
					// touche clavier
					case SDLK_RETURN:
						menu_b = true;
						break;

					case SDLK_ESCAPE:
            end_b = true;
            break;
					default:
            break;
				}
				break;

				//printf("mouse click %d \n", event.button.button);
				break;
			default: break;
			}
		}

		SDL_SetRenderDrawColor(renderer, 0,0,0,255);
		SDL_RenderClear(renderer);




		SDL_RenderCopy(renderer, background, NULL, NULL);



		// TEXT
		SDL_Color White = {255, 255, 255, 255};


		SDL_Rect Message_rect;

		// DEFEAT
		surface = TTF_RenderText_Solid(font, "GAME OVER" , White);
		texture = SDL_CreateTextureFromSurface(renderer, surface);

		Message_rect.w = 500;
		Message_rect.h = 150;
		Message_rect.x = (WIDTH-Message_rect.w)*0.5;
		Message_rect.y = (HEIGHT-Message_rect.h)*0.2;

		SDL_RenderCopy(renderer, texture, NULL, &Message_rect);

		// PRESS SCORE


		surface = TTF_RenderText_Solid(font, "YOUR SCORE" , White);
		texture = SDL_CreateTextureFromSurface(renderer, surface);

		Message_rect.w = 400;
		Message_rect.h = 75;
		Message_rect.x = (WIDTH-Message_rect.w)*0.5;
		Message_rect.y = (HEIGHT-Message_rect.h)*0.5;

		SDL_RenderCopy(renderer, texture, NULL, &Message_rect);


		// PRESS RESULT
		char buf[256];
		sprintf(buf, "%u", score);

		surface = TTF_RenderText_Solid(font, buf , White);
		texture = SDL_CreateTextureFromSurface(renderer, surface);

		Message_rect.w = 250;
		Message_rect.h = 50;
		Message_rect.x = (WIDTH-Message_rect.w)*0.5;
		Message_rect.y = (HEIGHT-Message_rect.h)*0.65;

		SDL_RenderCopy(renderer, texture, NULL, &Message_rect);

		// PRESS CONTINUE


		surface = TTF_RenderText_Solid(font, "PRESS ENTER TO CONTINUE" , White);
		texture = SDL_CreateTextureFromSurface(renderer, surface);

		Message_rect.w = 400;
		Message_rect.h = 75;
		Message_rect.x = (WIDTH-Message_rect.w)*0.5;
		Message_rect.y = (HEIGHT-Message_rect.h)*0.9;

		if(fmod(SDL_GetTicks(),500) > 50) {
			SDL_RenderCopy(renderer, texture, NULL, &Message_rect);
		}

		SDL_RenderPresent(renderer);

		// SOUND

		if(audio_len == 0) {
			audio_pos = s.audio_pos;
			audio_len = s.audio_len;
		}

		SDL_Delay(DELAY);

  }
	if(menu_b) {
		ss.advance(false);
		ss.turnLeft(false);
		ss.turnRight(false);
		shoot = false;

		score = 0;

		Spaceship new_ss;
		ss = new_ss;

		vector<Asteroid> a_list_new;
		a_list = a_list_new;

		vector<Shot> s_list_new;
		s_list = s_list_new;

		Medikit m;
		medikit = m;

		menu();
	}

};

void Game::start(){
  bool quit = false;

  while (!quit && !end_b) {
		SDL_Event event;
		while (!quit && SDL_PollEvent(&event)) {
			switch (event.type) {
				case SDL_QUIT:
					quit = true;
					break;

				case SDL_KEYDOWN:
					switch (event.key.keysym.sym) {
						case SDLK_SPACE:
							if(!shoot) {
								new_shot(ss.getHead(), ss.getAngle());
								shoot = true;
							}
							break;

						case SDLK_RIGHT:
	            ss.turnRight(true);
							break;

						case SDLK_LEFT:
							ss.turnLeft(true);
							break;

						case SDLK_UP:
							ss.advance(true);
							break;



	          case SDLK_DOWN:
							ss.teleport();
	            break;

						case SDLK_ESCAPE:
	            quit = true;
	            break;

						default:
	            break;
					}
					break;

				case SDL_KEYUP:
					switch (event.key.keysym.sym) {
						case SDLK_SPACE:
							shoot = false;
							break;

						case SDLK_RIGHT:
							ss.turnRight(false);
							break;

						case SDLK_LEFT:
							ss.turnLeft(false);
							break;

						case SDLK_UP:
							ss.advance(false);
							break;

						default:
	            break;
					}
					break;

			default:
				break;
			}
		}

    update();
    SDL_Delay(DELAY);
  }
};

void Game::update() {

	SDL_SetRenderDrawColor(renderer, 0,0,0,255);
	SDL_RenderClear(renderer);

	SDL_RenderCopy(renderer, background, NULL, NULL);
	//Spaceship


	ss.move();
	ss.wrap();
	ss.display(renderer);

	unsigned int size;


  // Shots
  size = s_list.size();
  for (int i(size-1) ; i > -1 ; i--) {
    if (s_list[i].getNb() < DISTANCE_SHOT) {
    s_list[i].incrNb();
    s_list[i].move();
    s_list[i].wrap();
    s_list[i].display(renderer);
    }
    else {
      s_list.erase(s_list.begin() + i);
    }
  }

  // Asteroids
  size = a_list.size();
  for (int i(size-1) ; i > -1 ; i--) {
    a_list[i].move();
    a_list[i].wrap();
    a_list[i].display(renderer);
  }


  // Test collision

  unsigned int size_a_list, size_s_list, size_ss_list;
  vector<Point> s_l, a_l,ss_l;

  size_s_list = s_list.size();
  size_a_list = a_list.size();



  // Shot && Asteroid
  bool erase = false;
  float R1, R2;
  Point center;

  for (int i(size_s_list-1) ; i > -1 ; i--) {
    for (int j(size_a_list-1) ; !erase && j > -1 ; j--) {
      a_l = a_list[j].getList();
      center = a_list[j].getCenter();
      R1 = distance(center, a_l[1]);
      R2 = distance(center, s_list[i].getCenter());
      if (R1 >= R2) {
        erase = true;
				int l = a_list[j].getLevel();
				Point p = a_list[j].getCenter();
				if(l == BIG){
					new_asteroid(MEDIUM, 2, p);
				}
				else if (l == MEDIUM){
					new_asteroid(SMALL, 2, p);
				}
				score += a_list[j].getPoint();
				a_list.erase(a_list.begin() + j);
      }
    }
    if(erase) {
      s_list.erase(s_list.begin() + i);
      erase = false;
    }
  }


	// Invincible ?
	int life;

	if (ss.getInvincible() == true && SDL_GetTicks() - ss.getTime() > INV_TIME) {
		ss.switchInvincible();
	}

	// Item && Spaceship
	ss_l = ss.getList();
	size_ss_list = ss_l.size();

	bool med_b = medikit.getAlready();
	if (!med_b) {
		medikit.testNew();
	}


	medikit.display(renderer);

	int len;
	if (med_b) {
		center = medikit.getCenter();
		len = medikit.getLen();
		Point p(center.getX()-len/(2*3), center.getY()-len/2);


		R1 = distance(center, p);
		R2 = distance(center, ss_l[0]);

		for (unsigned int i(1) ; i < size_ss_list;i++) {
			float tmp = distance(center, ss_l[i]);
			if (tmp < R2){
				R2 = tmp;
			}
		}

		if(R1 >= R2) {
			medikit.switchAlready();
			ss.setLife(ss.getLife()+MEDIKIT_VALUE);

			Medikit m;
			medikit = m;
		}

	}





  // Asteroid && Spaceship
	size_a_list = a_list.size();

	for (unsigned int i(0) ; !ss.getInvincible() && i < size_a_list ; i++) {
		center = a_list[i].getCenter();
		a_l = a_list[i].getList();
		R1 = distance(center, a_l[1]);
		R2 = distance(center, ss_l[0]);

		for (unsigned int i(1) ; i < size_ss_list;i++) {
			float tmp = distance(center, ss_l[i]);
			if (tmp < R2){
				R2 = tmp;
			}
		}

		if (R1 >= R2) {
			life = ss.getLife();
			if(life > 0)  {
				ss.switchInvincible();
				ss.setTime(SDL_GetTicks());
				ss.setLife(life-ASTEROID_DAMAGE);
			}
		}
	}



	// DEAD ?
	if (ss.getLife() <= 0) end();

	// NB AST MIN ?
	if (a_list.size() < NB_AST_MIN) {
		new_asteroid(BIG, NB_AST_MIN - a_list.size());
	}

	updateScore(ss.getLife());
  SDL_RenderPresent(renderer);

	// SOUND
	if(audio_len == 0) {
		audio_pos = s.audio_pos;
		audio_len = s.audio_len;
	}

};
