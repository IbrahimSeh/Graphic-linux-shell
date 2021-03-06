/*
 * History.h
 *
 *  Created on: Jan 9, 2019
 *      Author: root
 */

#ifndef MENU_H_
#define MENU_H_
#define HISTORY_DEFAULT_SIZE 100
#include <string>
#include <menu.h>
class Menu {
public:
	Menu(int count, std::string *stringArray);
	std::string showMenu();

	ITEM **my_items;
	MENU *my_menu;
	WINDOW *my_menu_win;
private:
	void unpostAndFree(MENU *my_menu, ITEM **my_items);
	void initMenu();
	void endMenu();
	int mouseClick(WINDOW *win, MEVENT *mouseEvent);
	void initItems(std::string *stringArray);
	int count;
};

#endif /* MENU_H_ */
