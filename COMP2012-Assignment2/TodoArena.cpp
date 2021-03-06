/*
 * Arena.cpp
 *
 *  Created on: Feb 25, 2016
 *      Author: kevinw
 */

#include "Arena.h"

#include "Enemy.h"
#include "TodoCatapult.h"
#include "TodoDiagonalEnemy.h"
#include "TodoIceTower.h"
#include "TodoLaserTower.h"
#include "TodoRevivingEnemy.h"
#include "TodoSimpleTower.h"
#include <fstream>

const int ARENA_W = 45;
const int ARENA_H = 5;
const int TOWER_X = 35;
const int ENEMY_X = 0;




Arena::~Arena() {
//TODO
	/*for(int i=0; i<num_objects; i++)
	{
		delete []objects[i];
	}*/
	delete []objects;
}

//Perform shadow copying and add this object to the array objects.
//The ownership of this object is transfered to Arena.
void Arena::addObject(Object* newobj) {
//TODO
	Object** temp=new Object*[num_objects];
	for(int i=0; i<num_objects; i++)
	{
		temp[i]=objects[i];
	}
	num_objects+=1;
	objects=new Object*[num_objects];
	for(int i=0; i<num_objects; i++)
	{
		if(i==num_objects-1)
		{
			objects[i]=newobj;
			break;
		}
		objects[i]=temp[i];
	}
	delete []temp;
}

//To return a dynamic const Object array. You might treat the memory
//pointed by obj is garbage.
void Arena::getConstObjects(const Object**& obj, int& count) const {
//TODO
	count=num_objects;
	obj=new const Object*[num_objects];
	for(int i=0; i<num_objects; i++)
	{
		obj[i]=objects[i];
	}

}

//Return the object in the array objects with the coordinate aX and aY.
//If there are more than one object is in this coordinate, return any
//one of them. If there are no object is in this coordinate, return nullptr 
Object* Arena::getObjectAt(int aX, int aY) const {
//TODO
	int a,b;
	for(int i=0; i<num_objects; i++)
	{
		objects[i]->getXY(a,b);
		if(aX==a && aY==b)
		{
			return objects[i];
		}
	}
	return nullptr;

}

//Return true if the game is over, false otherwise.
//The game is over when any not DEAD enemy reaches the end-zone - i.e., its
//x-coordinate >= the constant TOWER_X.
bool Arena::isGameOver() const {
//TODO

	for(int i=0; i<ARENA_H; i++)
	{
		if(getObjectAt(TOWER_X,i)==nullptr)
		{
			return false;
		}
	}
	return true;
}


//Remove the object pointed by the pointer r from the array objects if it is 
//in the list. Do nothing if r is a nullptr or r is not in the array.
void Arena::removeObject(Object* r) {
//TODO
	if(r==nullptr)
	{
		return;
	}
}


//Depends on the value of the variable building, construct a tower at the coordinate x, y when money is larger than the building cost.
// if building is SIMPLE_TOWER - construct a TodoSimpleTower
// if building is CATAPUL_TOWER - construct a TodoCatapult
// if building is LASER_TOWER - construct a TodoLaserTower
// if building is ICE_TOWER - construct a TodoIceTower
// otherwise - do nothing.
// After building the tower, deduct the money by the building cost of the tower
void Arena::addBuilding(int building, int x, int y) {
//TODO
	//Tower* temp;
	/*switch (building)
	{
		case SIMPLE_TOWER:
			temp = new TodoSimpleTower(x,y);
			money-=1;
			break;
		case CATAPULT_TOWER:
			temp = new TodoCatapultTower(x,y);
			money-=2;
			break;
		case LASER_TOWER:
			temp = new TodoLaserTower(x,y);
			money-=3;
			break;
		case ICE_TOWER:
			temp = new TodoIceTower(x,y);
			money-=3;
			break;
		default:
			return;
		return;
	}
	addObject(temp);*/
	Tower* temp;
	if(building==SIMPLE_TOWER)
	{
		if(money>=1)
		{
			temp=new TodoSimpleTower(x,y);
			money-=1;
			addObject(temp);
		}
		return;
	}
	else if(building==CATAPULT_TOWER)
	{
		if(money>=2)
		{
			temp=new TodoCatapultTower(x,y);
			money-=2;
			addObject(temp);
		}
		return;
	}
	else if(building==LASER_TOWER)
	{
		if(money>=3)
		{
			temp=new TodoLaserTower(x,y);
			money-=3;
			addObject(temp);
		}
		return;
	}
	else if(building==ICE_TOWER)
	{
		if(money>=3)
		{
			temp=new TodoIceTower(x,y);
			money-=3;
			addObject(temp);
		}
		return;
	}
	else
	{
		return;
	}
}


//This function would first make all tower fires (if there is 
//at least one enemy in its range). By default, the tower will select 
//the nearest enemy to shoot. If there are more than one nearest enemies,
//pick anyone of them (we would not test this case during grading).
//
//Next all enemies are going to move. Remember that an enemy in the state
//FROZEN or DEAD cannot move.  
//
//Next you need to generate one new Enemy by calling the function genereateEnemy(). The function generateEnemy has been written for you already.
//
//Finally you need to clean up all DEAD enemy and tower that was destroyed by enemy in this turn.
//
//The amount of money will be increased by the enemies killed in this turn.
//(*noted: if a RevivingEnemy revive, money will not be added) 
void Arena::nextRound() {
//TODO

	bool temp;
	/*for(int i=0; i<num_objects; i++)
	{
		ObjectType x=objects[i]->getObjectType();
		if(x==ObjectType::TOWER)
		{
			for(int k=0; k<num_objects; k++)
			{
				ObjectType y=objects[i]->getObjectType();
				int a,b;
				objects[i]->getXY(a,b);
				if(y==ObjectType::ENEMY)
				{
					//temp=objects[i]->isInRange(a,b);
				}
			}
			if(temp==true)
			{
				//objects[i]->fire();
			}
		}
	}*/

	for(int i=0; i<num_objects; i++)
	{
		ObjectType x=objects[i]->getObjectType();
		if(x==ObjectType::ENEMY)
		{
			dynamic_cast<Enemy*>(objects[i])->move();
		}
	}

	generateEnemy();
}


//Completed
bool Arena::upgrade(Tower* t) {
	if (t == nullptr || money < t->getUpgradeCost())
		return false;
	money -= t->getUpgradeCost();
	t->upgrade();
	return true;
}

//Completed
void Arena::generateEnemy() {

	int i = rand() % ARENA_H;
	Enemy* e;
	switch (rand() % 3){
	case 0: e = new Enemy(5, ENEMY_X, i);  break;
	case 1: e = new TodoDiagonalEnemy(6, ENEMY_X, i);  break;
	case 2: e = new TodoRevivingEnemy(10, ENEMY_X, i);  break;
	}
	addObject(e);

}

//Completed
Arena::Arena() : objects(nullptr), num_objects(0), money(10), enemy_kill(0) {
	nextRound();
}
