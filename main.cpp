#include <SFML/Graphics.hpp>  //Библиотека для отрисовки
#include "set.h"
#include <iostream>
#include <vector>
#include <cmath>
#include <complex>

#define ANTI_ALIASING 8
#define WID_DEF 800

using namespace std;
using namespace sf;

RenderWindow window(VideoMode(WIDTH, HEIGHT), "Circle paint", Style::Close, ContextSettings(0,0,ANTI_ALIASING));

//vector <double> PIXELS_POSITION;
vector <complex <double>> PIXELS_POSITION;
vector <complex <double> > CONSTANTS;

complex <double> TEMP_COMPLEX;
complex <double> imaginary_i(0, 1);

Font MAIN_FONT;
Text TEXT_WHITE_ALPHA;

//double FUNC[WID_DEF + 1];

Vertex LINE_DOT[] =
	{
		Vertex(Vector2f(0.f, 0.f)),
		Vertex(Vector2f(1.f, 1.f))  
	};	

double Weer(float a, float b, double x){
	float k, m, y = 0;
	for(int i = 0; i < 100; i++){
	   k = cos(M_PI * x * pow(a, i));
	   m = pow(b, i);
	   y += k * m;
	}
	return y;
}

void Start(){
	//double hd = HEIGHT/4;
	PIXELS_POSITION.resize(WIDTH + 1);
	for(double i=0; i<=WIDTH; i++){
		//PIXELS_POSITION[i] = complex <double>(0, 65 * Weer(2, 0.5, i/65)); //y
		PIXELS_POSITION[i] = complex <double>(0, i - HEIGHT/2);
	}
	
	
	MAIN_FONT.loadFromFile("arkhip.ttf");
	
	TEXT_WHITE_ALPHA.setFont(MAIN_FONT);
	TEXT_WHITE_ALPHA.setFillColor(Color(255,255,255,128)); //100
	TEXT_WHITE_ALPHA.setCharacterSize(20);
}

void Connect_Dot(float x1, float y1, float x2, float y2, Color Cl){	
	LINE_DOT[0].position = Vector2f(x1, y1);
	LINE_DOT[1].position = Vector2f(x2, y2);
	
	LINE_DOT[0].color = Cl;
	LINE_DOT[1].color = Cl;
	
	window.draw(LINE_DOT, 2, Lines);  
}

int Circle_number(int i)
{
  if (i%2) 
  	return 1 + int(i/2); 

  else 
  	return -i/2;
}

void Render(){
	int hg2 = HEIGHT/2, wd2 = WIDTH/2;
	switch(SCENE){
		case 1:{
			double x, x0 = 0, y, y0 = PIXELS_POSITION[0].imag();
			for(int i = 0; i <= WIDTH; i++) 
			{
				x = i;
				y = PIXELS_POSITION[i].imag();
	            Connect_Dot(x0, hg2 - y0, x, hg2 - y, Color(250,180,180));
	            x0 = i;
				y0 = y;
	        }	        
	    	break;
		}
		case 2:{
			//===========
			double x, x0 = 0, y, y0 = PIXELS_POSITION[0].imag();
			for(int i = 0; i <= WIDTH; i++) 
			{
				x = i;
				y = PIXELS_POSITION[i].imag();
	            Connect_Dot(x0, hg2 - y0, x, hg2 - y, Color(250,180,180));
	            x0 = i;
				y0 = y;
				//cout<<i<<" "<<y<<"\n";
	        }
	        //============
			x, x0 = -1, y, y0 = 0; //double
			
			for(MAIN_TIME = 0; MAIN_TIME < 1; MAIN_TIME += DELTA_TIME) { //x - main time
				y = 0;
				//x = 0;
				for(int i = 1; i <= CIRCLES_KOL; i++) {	
					TEMP_COMPLEX = CONSTANTS[i] * exp(imaginary_i * MAIN_TIME * double(Circle_number(i)) * 2. * M_PI);
					
					y += TEMP_COMPLEX.imag();
					//x += TEMP_COMPLEX.real();	
				}

				x = MAIN_TIME * WID_DEF;
				Connect_Dot(x0, hg2 - y0, x, hg2 - y, Color::White);
				x0 = x;
				y0 = y;
				//cout<<x<<"]["<<y<<"\n";
			}
			TEXT_WHITE_ALPHA.setString("Number waves: " + to_string(CIRCLES_KOL));
			TEXT_WHITE_ALPHA.setPosition(5, HEIGHT - 25);
			window.draw(TEXT_WHITE_ALPHA);
			
			break;
		}
	}
}

void Calc_const(){
	DELTA_TIME = 1./(WID_DEF+1);
	CIRCLES_KOL = WID_DEF + 1;
	int sz = PIXELS_POSITION.size()-1;
	
	if(CONSTANTS.size() < 1)
		for(int i = 0; i <= CIRCLES_KOL; i++) 
			CONSTANTS.push_back((0, 0));
	
	for(int i = 1; i <= CIRCLES_KOL; i++){
		//CONSTANTS.push_back((0, 0)); //DELTA_TIME/2.
		for(MAIN_TIME = 0; MAIN_TIME < 1; MAIN_TIME += DELTA_TIME) {
			//cout<<MAIN_TIME * sz<<endl;
	       	CONSTANTS[i] +=  PIXELS_POSITION[round(MAIN_TIME * sz)] * exp(double(-Circle_number(i)) * M_PI * 2.*imaginary_i * MAIN_TIME);
		}

		CONSTANTS[i] *= DELTA_TIME;
		//cout<<CONSTANTS[i]<<endl;
	}
	CIRCLES_KOL = 1;
}

//inline

int main()
{		
	Start();
	
	int wd2 = WIDTH/2, hg2 = HEIGHT/2;
	
	// окно нельзя разворачивать !!! для пэинта
	while (window.isOpen())  
	{	
		Event event;
		while (window.pollEvent(event))  //Если произошло событие
		{
			if (event.type == Event::Closed){  //Если событие - закртыие окна
				window.close();  //Закрываем окно
			}
			
            switch(SCENE){
				case 1:{ //========================
					if(event.type == sf::Event::KeyPressed){
		            	/*if(event.key.code == sf::Keyboard::R){
			            	PIXELS_POSITION.clear();
							BUTTON = '-';
							CLICK = false;
						}*/
						if(event.key.code == sf::Keyboard::Enter){
			            	SCENE = 2;
			            	BUTTON = '-';
							CLICK = false;							
							Calc_const();	
						}
					}
				
					if ((event.type == Event::MouseButtonPressed or event.type == Event::MouseButtonReleased) and event.mouseButton.button == sf::Mouse::Left)
					{
					    int x, y;	
					    x = event.mouseButton.x;
						y = event.mouseButton.y;
						if(0 <= x and x <= WIDTH)
							CLICK = !CLICK;
						else
							CLICK = false;
						
						//cout<<CLICK<<" LF "<<x<<" "<<y<<endl;
		
						if(CLICK){
							BUTTON = 'L';
							LAST_X = x;
							LAST_Y = y;
							PIXELS_POSITION[x] = complex <double>(0, round(hg2 - y));
						}
						else
							BUTTON = '-';	
					}//========================
					if (event.type == sf::Event::MouseMoved){
						if(CLICK and BUTTON == 'L'){
							float x = event.mouseMove.x, y = event.mouseMove.y, delta, plus; 	
							
						    if(0 <= x and x <= WIDTH){
						    	if(abs(LAST_X - x) <= 1)
						   			PIXELS_POSITION[x] = complex <double>(0, round(hg2 - y));
						   		else{
						   			if(abs(x - LAST_X) >= abs(y - LAST_Y)){
										delta = (y - LAST_Y)/abs(x - LAST_X); 
										plus = (x - LAST_X)/abs(x - LAST_X);
										
										x -= 0.5;
										y -= 0.5;
										
										while(LAST_X <= x or LAST_X >= x + 1){
											//cout<<LAST_X<<" "<<LAST_Y<<"\n"; 
											PIXELS_POSITION[round(LAST_X)] = complex <double>(0, round(hg2 - LAST_Y));
											LAST_X += plus;
											LAST_Y += delta;
										}
										PIXELS_POSITION[round(x)] = complex <double>(0, round(hg2 - y));
									}
									else{
										delta = (x - LAST_X)/abs(y - LAST_Y); 
										plus = (y - LAST_Y)/abs(y - LAST_Y);
										
										x -= 0.5;
										y -= 0.5;
										
										while(LAST_Y <= y or LAST_Y>= y + 1){
											PIXELS_POSITION[round(LAST_X)] = complex <double>(0, round(hg2 - LAST_Y));
											LAST_Y += plus;
											LAST_X += delta;
										}
										PIXELS_POSITION[round(x)] = complex <double>(0, round(hg2 - y));
									}	
								}
						   		LAST_X = round(x);
								LAST_Y = round(y);
							}
						}
					}
				break;
				} 	//CIRCLES_KOL	
				case 2:{
					if(event.key.code == sf::Keyboard::R){
						BUTTON = '-';
						CLICK = false;
						SCENE = 1;
					}
					if(event.type == Event::MouseWheelMoved){					
						int x = event.mouseWheel.x, y = event.mouseWheel.y;
						
						if((0 <= x and x <= WIDTH) and (0 <= y and y <= HEIGHT)){
							CIRCLES_KOL += event.mouseWheel.delta;
							if(CIRCLES_KOL < 1)
								CIRCLES_KOL = 1;
							if(CIRCLES_KOL > WIDTH)
								CIRCLES_KOL = WIDTH;
						}	
					}
				break;
				}
			}
		}
				
		window.clear(Color(0, 137, 207)); 
		
		Render();
		
		window.display();
	}
}
