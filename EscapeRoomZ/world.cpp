#include <iostream>
#include "global_f.h"
#include "entity.h"
#include "creature.h"
#include "item.h"
#include "exit.h"
#include "room.h"
#include "player.h"
#include "world.h"



World::World(const char* p_name)
{
	ttimer = clock();
	Room* bedroom = new Room("Bedroom", "You are in your bedroom, but everything feels weird");
	Room* hall = new Room("Hall", "You got into a long creepy hall", true);
	Room* outside = new Room("Outside", "Outside your house");

	Exit* exit_bedroom = new Exit("right", "behind", "Door", bedroom, hall, RIGHT);
	Exit* exit_bedroom2 = new Exit("left", "right", "Window", bedroom, outside, LEFT);
	exit_bedroom2->closed = true; //window is closed and cant be opened

	Room* gameroom1 = new Room("Gameroom", "You are in a bright little room");
	Exit* exit_hall = new Exit("straight", "behind", "Main door", hall, outside, STRAIGHT);
	exit_hall->locked = true; //Main door locked
	Exit* exit_hall2 = new Exit("left", "behind", "Door", hall, gameroom1, LEFT, true);

	Room* gameroom2 = new Room("Gameroom", "You are in a bright little room");
	Room* gameroom8 = new Room("Gameroom", "You are in a dark little room... something starts beeping... ");
	Exit* exit_gr1 = new Exit("straight", "behind", "Door", gameroom1, gameroom2, STRAIGHT, true);
	Exit* exit_gr1b = new Exit("right", "behind", "Door", gameroom1, gameroom8, RIGHT, true);

	Room* gameroom3 = new Room("Gameroom", "You are in a bright little room");
	Room* gameroom4 = new Room("Gameroom", "You are in a bright little room");
	Room* gameroom5 = new Room("Gameroom", "You are in a bright little room");
	Room* gameroom6 = new Room("Gameroom", "You are in a bright little room");
	Room* gameroom7 = new Room("Gameroom", "You are in a bright little room");

	Room* gameroom9 = new Room("Gameroom", "You are in a dark little room... something starts beeping...");


	entities.push_back(bedroom);
	entities.push_back(hall);
	entities.push_back(outside);
	entities.push_back(gameroom1);
	entities.push_back(gameroom2);
	entities.push_back(gameroom3);
	entities.push_back(gameroom4);
	entities.push_back(gameroom5);
	entities.push_back(gameroom6);
	entities.push_back(gameroom7);
	entities.push_back(gameroom8);

	Item* flashlight = new Item("Flashlight", "Looks like it might contain something.", bedroom, TOOL);
	Item* key = new Item("Key", "Your house key", gameroom7);
	exit_hall->key = key;

	player = new Player(p_name, "New player", bedroom);

	entities.push_back(player);

}

World::~World()
{

}

bool World::Update(vector<string>& args)
{
	bool ret = true;

	if (args.size() > 0 && args[0].length() > 0)
		ret = ExecuteInteraction(args);

	UpdateGame();

	return ret;
}


void World::UpdateGame()
{
	clock_t now = clock();

	if ((now - ttimer) / CLOCKS_PER_SEC > TICK_FREQUENCY)
	{
		for (list<Entity*>::iterator it = entities.begin(); it != entities.end(); ++it)
			(*it)->Update();

		ttimer = now;
	}
}


bool World::ExecuteInteraction(vector<string>& args)
{
	bool end = true;

	switch (args.size())
	{
	case 1:
	{
		if (Cmp(args[0], "look") || Cmp(args[0], "l"))
		{
			player->Look(args);
		}
		else if (Cmp(args[0], "straight") || Cmp(args[0], "front"))
		{
			if (args.size() == 1)
			{
				args.push_back("straight");
			}
			else
			{
				args[1] = "straight";
			}
			player->Go(args);
		}

		else if (Cmp(args[0], "back"))
		{
			if (args.size() == 1)
			{
				args.push_back("back");
			}
			else
			{
				args[1] = "back";
			}
			player->Go(args);
		}
		else if (Cmp(args[0], "right"))
		{
			if (args.size() == 1)
			{
				args.push_back("right");
			}
			else
			{
				args[1] = "right";
			}
			player->Go(args);
		}
		else if (Cmp(args[0], "left") || Cmp(args[0], "l"))
		{
			if (args.size() == 1)
			{
				args.push_back("left");
			}
			else
			{
				args[1] = "left";
			}
			player->Go(args);
		}
		else if (Cmp(args[0], "inventory") || Cmp(args[0], "i"))
		{
			player->Inventory();
		}
		else
			end = false;
		break;
	}
	case 2:
	{
		if (Cmp(args[0], "look") || Cmp(args[0], "l"))
		{
			player->Look(args);
		}
		else if (Cmp(args[0], "go"))
		{
			player->Go(args);
		}
		else if (Cmp(args[0], "take") || Cmp(args[0], "pick") || Cmp(args[0], "get"))
		{
			player->Take(args);
		}
		else if (Cmp(args[0], "drop") || Cmp(args[0], "put"))
		{
			player->Drop(args);
		}
		else if (Cmp(args[0], "equip") || Cmp(args[0], "eq") || Cmp(args[0], "use"))
		{
			player->Equip(args);
		}
		else if (Cmp(args[0], "unequip") || Cmp(args[0], "uneq"))
		{
			player->UnEquip(args);
		}

		else
			end = false;
		break;
	}
	case 3:
	{
		break;
	}
	case 4: // commands with three arguments ------------------------------
	{
		if (Cmp(args[0], "unlock") || Cmp(args[0], "unlk"))
		{
			player->UnLock(args);
		}
		else if (Cmp(args[0], "take") || Cmp(args[0], "pick"))
		{
			player->Take(args);
		}
		else if (Cmp(args[0], "drop") || Cmp(args[0], "put"))
		{
			player->Drop(args);
		}
		else
			end = false;
		break;
	}
	default:
		end = false;
	}

	return end;
}