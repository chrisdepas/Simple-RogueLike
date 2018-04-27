#ifndef __ACTION_H__
#define __ACTION_H__

enum EAction
{
	/* Special actions */
	Action_None, // No action - do nothing
	Action_Unknown, // Unknown action, returned on error

	/* Movement actions */
	Action_Move_Forward,
	Action_Move_Back,
	Action_Move_Left,
	Action_Move_Right,
	Action_Move_Up, // Up, e.g. ladder
	Action_Move_Down, // Down e.g. down stairs

	/* Menu */
	Action_Menu_Toggle,
	Action_Menu_Select,

	/* Gameplay */
	Action_Grab, // Pick up item(s) on current tile 
	Action_Use, // Activate a tile 
	Action_Fire, // Fire current weapon 
	Action_Reload, //Reload current weapon
	Action_Inventory, // Open Inventory 
	Action_Drop, // Drop an item 
	Action_Eat, // Eat an item
	Action_Equip, // Equip an item
	Action_Wait, //Waste turns
	Action_Throw, //Throw an item
};

#endif