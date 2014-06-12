#include "sdl.h"
#include <string.h>

bool juego = true;
SDL_Event event;
const int cant_disparos = 25;
const int vel_nave = 10;
SDL_Surface* pantalla = NULL;
SDL_Surface* fondo = NULL;
SDL_Surface* marco = NULL;
SDL_Surface* nave_principal = NULL;
SDL_Surface* barra_lateral = NULL;
SDL_Surface* enemigo = NULL;
SDL_Surface* item_menu1 = NULL;
SDL_Surface* item_menu2 = NULL;
SDL_Surface* item_menu3 = NULL;
SDL_Surface* item_menu4 = NULL;
SDL_Surface* item_menu5 = NULL;
SDL_Surface* disparo[cant_disparos];
// rectángulo nave principal
SDL_Rect rect_nave;
// rectángulo disparo
SDL_Rect pos_disparo[cant_disparos];
// rectángulo enemigos
SDL_Rect pos_enemigo;
// banderas para gestionar movimientos
bool f_right = false;
bool f_left = false;
bool f_up = false;
bool f_down = false;
bool f_disparar = false;

// posicion horizontal de la nave
int posicion_nave_x = 460;
int posicion_nave_y = 543;
// posicion de los enemigos
int posicion_obst_x = 0;
int posicion_obst_y = 0;
//posicion disparo
int posicion_disparo_y = posicion_nave_y;
int posicion_disparo_x;
// contador para diferenciar los tiempos entre las animaciones
unsigned int cont_animacion;
int contador_barra = 1;
int contador_disparo = 0;
int vel_obst = 0;
int vel_obst_x = 85;



//manejador eventos
void gestionarEventos()
{
	switch(event.type)
	{
	case SDL_QUIT:
		juego = false;
		break;
	case SDL_KEYDOWN:
		switch (event.key.keysym.sym)
		{
		case SDLK_ESCAPE:
			juego = false;
			break;
		case SDLK_SPACE:
 			contador_disparo ++;
			// se le asigna la posición de la nave al disparo
			if(contador_disparo > cant_disparos)
			{
				contador_disparo = 1;
			}
			f_disparar = true;
			pos_disparo[contador_disparo-1].x = posicion_nave_x + 33;
			pos_disparo[contador_disparo-1].y = posicion_nave_y;
			
			break;
		case SDLK_LEFT:
			f_left = true;
			break;
		case SDLK_RIGHT:
			f_right = true;
			break;
		case SDLK_UP:
			f_up = true;
			contador_barra--;
			if(contador_barra < 1)
				contador_barra = 1;
			break;
		case SDLK_DOWN:
			f_down = true;
			contador_barra++;
			if(contador_barra> 5)
				contador_barra = 5;
			break;
		case SDLK_RETURN:
			break;
		default:
			break;
		}
		break;
		// cuando se deja de presionar la tecla se desactivan las banderas
	case SDL_KEYUP: 
		switch (event.key.keysym.sym)
		{
		case SDLK_LEFT:
			f_left = false;
			break;
		case SDLK_RIGHT:
			f_right = false;
			break;
		case SDLK_UP:
			f_up = false;
			break;
		case SDLK_DOWN:
			f_down = false;
			break;
		default:
			break;
		}
		break;
	default:
		break;
	}
}

void inicializarJuego()
{
	//inicializar
	SDL_Init(SDL_INIT_EVERYTHING);

	//cargar
	//pantalla = SDL_SetVideoMode(1004,714,32,SDL_SWSURFACE);
	pantalla = SDL_SetVideoMode(1004,714,32,SDL_FULLSCREEN);
	SDL_WM_SetCaption("Inner Ship", 0);
	fondo = SDL_LoadBMP("recursos/imagenes/fondo.bmp");
	marco = SDL_LoadBMP("recursos/imagenes/marco.bmp");
	nave_principal = SDL_LoadBMP("recursos/imagenes/ns.bmp");
	barra_lateral = SDL_LoadBMP("recursos/imagenes/barra_lateral1.bmp");
	enemigo = SDL_LoadBMP("recursos/imagenes/asteroide.bmp");
	

	//definiendo transparencia
	SDL_SetColorKey(barra_lateral,SDL_SRCCOLORKEY,SDL_MapRGB(barra_lateral->format, 255,0,255));
	//SDL_SetColorKey(nave_principal,SDL_SRCCOLORKEY,SDL_MapRGB(nave_principal->format, 255,0,255));
	//SDL_SetColorKey(enemigo,SDL_SRCCOLORKEY,SDL_MapRGB(enemigo->format, 255,0,255));
	SDL_SetColorKey(marco,SDL_SRCCOLORKEY,SDL_MapRGB(marco->format, 255,0,255));
	
}

bool colision(SDL_Rect a, SDL_Rect b)
{
	if(a.x > (b.x+b.w))
	{
		return false;
	}else
	if(a.y > (b.y+b.h))
	{
		return false;
	}else
	if((a.x+b.w) < b.x)
	{
		return false;
	}else
	if((a.y+a.h) < b.y)
	{
		return false;
	}else
	{
		SDL_BlitSurface(nave_principal,NULL, pantalla,&b);

		return true;
	}
}

void disparar()
{
	for(int i = 0; i< contador_disparo; i++)
	{
		pos_disparo[i].y -= 20;
		if( pos_disparo[i].y < -30)
		{
			pos_disparo[i].y = posicion_nave_y;
			f_disparar = false;
		}
		if(contador_barra == 1)
		{
			disparo[i] = SDL_LoadBMP("recursos/imagenes/disparo1.bmp");
			SDL_SetColorKey(disparo[i],SDL_SRCCOLORKEY,SDL_MapRGB(disparo[i]->format, 208,0,0));
		}
		if(contador_barra == 2)
		{
			disparo[i] = SDL_LoadBMP("recursos/imagenes/disparo2.bmp");
			SDL_SetColorKey(disparo[i],SDL_SRCCOLORKEY,SDL_MapRGB(disparo[i]->format, 208,169,0));
		}
		if(contador_barra == 3)
		{
			disparo[i] = SDL_LoadBMP("recursos/imagenes/disparo3.bmp");
			SDL_SetColorKey(disparo[i],SDL_SRCCOLORKEY,SDL_MapRGB(disparo[i]->format, 0,0,208));
		}
		if(contador_barra == 4)
		{
			disparo[i] = SDL_LoadBMP("recursos/imagenes/disparo4.bmp");
			SDL_SetColorKey(disparo[i],SDL_SRCCOLORKEY,SDL_MapRGB(disparo[i]->format, 208,0,162));
		}
		if(contador_barra == 5)
		{
			disparo[i] = SDL_LoadBMP("recursos/imagenes/disparo5.bmp");
			SDL_SetColorKey(disparo[i],SDL_SRCCOLORKEY,SDL_MapRGB(disparo[i]->format, 208,15,0));
		}
		colision(pos_disparo[i], pos_enemigo);
		SDL_BlitSurface(disparo[i],NULL, pantalla,&pos_disparo[i]);
	}
}



void gestionarImagenes()
{
	
	SDL_Rect pos_barra;
	SDL_Rect animation;
	SDL_Rect fase_nave;
	SDL_Rect pos_nave;

	// aplicar fondo
	SDL_BlitSurface(fondo,NULL,pantalla,NULL);
	

	//posicion nave principal
	if(f_left)
	{
		if(posicion_nave_x <= 85)
		{
			posicion_nave_x = 85;
		}
		else
		{
			posicion_nave_x -=vel_nave;
		}
	}
	if(f_right)
	{
		if(posicion_nave_x >= 825)
		{
			posicion_nave_x = 825;
		}
		else
		{
			posicion_nave_x +=vel_nave;
		}
	}

	//mostrar nave principal y seleccionar solo mostrar
	// una parte de esta
	pos_nave.x = posicion_nave_x;
	pos_nave.y = posicion_nave_y;
	fase_nave.x = 0;
	fase_nave.y = 0;
	fase_nave.h = 106;
	fase_nave.w = 102;
	

	//mostrar barra lateral
	pos_barra.x = 927;
	pos_barra.y = 250;

	//posicion barra lateral
	if(f_down || f_up || 1==1)
	{
		if(contador_barra == 1)
		{
			fase_nave.x = 0;		
			barra_lateral = SDL_LoadBMP("recursos/imagenes/barra_lateral1.bmp");
			SDL_SetColorKey(barra_lateral,SDL_SRCCOLORKEY,SDL_MapRGB(barra_lateral->format, 255,0,255));
		}
		if(contador_barra == 2)
		{
			fase_nave.x =+ 106;
			barra_lateral = SDL_LoadBMP("recursos/imagenes/barra_lateral2.bmp");
			SDL_SetColorKey(barra_lateral,SDL_SRCCOLORKEY,SDL_MapRGB(barra_lateral->format, 255,0,255));
		}
		if(contador_barra == 3)
		{
			fase_nave.x =+ 106*2;
			barra_lateral = SDL_LoadBMP("recursos/imagenes/barra_lateral3.bmp");
			SDL_SetColorKey(barra_lateral,SDL_SRCCOLORKEY,SDL_MapRGB(barra_lateral->format, 255,0,255));
		}
		if(contador_barra == 4)
		{
			fase_nave.x =+ 106*3;
			barra_lateral = SDL_LoadBMP("recursos/imagenes/barra_lateral4.bmp");
			SDL_SetColorKey(barra_lateral,SDL_SRCCOLORKEY,SDL_MapRGB(barra_lateral->format, 255,0,255));
		}
		if(contador_barra == 5)
		{
			fase_nave.x =+ 106*4;
			barra_lateral = SDL_LoadBMP("recursos/imagenes/barra_lateral5.bmp");
			SDL_SetColorKey(barra_lateral,SDL_SRCCOLORKEY,SDL_MapRGB(barra_lateral->format, 255,0,255));
		}
	}
	

	//// se toma en cuenta un periodo de tiempo antes actualizar la imagen de los enemigos
	//// para dar el efecto de la animación
	if((SDL_GetTicks() - cont_animacion) > 66.6)
	{
		// cambiar la posicion_obst_x para hacer el efecto de la animación
		// 512 es el tamaño de la imagen
		if(posicion_obst_x <= 512)
		{
			posicion_obst_x += 128;
		}
		if(posicion_obst_x == 512)
		{
			posicion_obst_x = 0;
			posicion_obst_y += 128;
		}
		if(posicion_obst_y == 512)
		{
			posicion_obst_y = 0;
		}
		// se resetea la variable en cada vuelta para que se pueda actualizar correctamente la imagen
		cont_animacion = SDL_GetTicks();
	}
	vel_obst = vel_obst + 20;
	
	if(vel_obst > 700)
	{
		vel_obst = 0;
		
		vel_obst_x = rand();

		while(vel_obst_x < 85 || vel_obst_x > 827)
		{
			vel_obst_x = rand();
		}
	}
	
	//mostrar enemigo
	pos_enemigo.x = vel_obst_x;
	pos_enemigo.y = vel_obst;
	animation.x = posicion_obst_x;
	animation.y = posicion_obst_y;
	animation.h = 128;
	animation.w = 128;
	SDL_BlitSurface(enemigo, &animation, pantalla,&pos_enemigo);

	//// manejar el disparo
	
	if(contador_disparo < cant_disparos)
	{
		disparar();
	}
	SDL_BlitSurface(marco,NULL,pantalla,NULL);
	SDL_BlitSurface(nave_principal, &fase_nave, pantalla,&pos_nave);
	SDL_BlitSurface(barra_lateral,NULL, pantalla,&pos_barra);
	colision(pos_nave, pos_enemigo);
}




int main(int argc, char* argv[])
{
	//tiempo inicial
	unsigned int tiempoInicial = SDL_GetTicks();
	cont_animacion = SDL_GetTicks();

	//inicializar video juego
	inicializarJuego();
	
	//loop principal
	while(juego)
	{
		//eventos
		while(SDL_PollEvent(&event))
		{
			gestionarEventos();
		}
		
		//aplicar imagenes
		gestionarImagenes();
		
		//refrescar la pantalla
		SDL_Flip(pantalla);

		//graduar tiempo
		while((tiempoInicial - SDL_GetTicks()) < 33.3);
	}
	SDL_FreeSurface(pantalla);
	SDL_Quit();
	return 0;
}


//if(f_disparar)
	//{
	//	/*posicion_disparo_y -= 10;

	//	rect.x = posicion_disparo_x;
	//	rect.y = posicion_disparo_y;
	//	SDL_BlitSurface(disparo[0],NULL, pantalla,&rect);

	//	if( posicion_disparo_y < -20)
	//	{
	//		posicion_disparo_y = posicion_nave_y;
	//		f_disparar = false;
	//	}*/

	//	disparar();
	//}