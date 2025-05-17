#ifndef GLOBAL_H
#define GLOBAL_H

#include <iostream>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <SFML/System.hpp>
#include <string>
#include <utility>
#include <cmath>
#include <cstring>
#include <cstddef>
#include <climits>
#include <map>
#include <fstream>
#include <unordered_set>
#include <stack>
#include <queue>
#include <GL/gl.h>
#include <GL/glut.h>

using namespace std;
using namespace sf;

#define catch_function(function) bool function(RenderWindow& window, Event& event, const RenderStates& state = RenderStates::Default)
#define _catch_function(class_name,function) bool class_name::function(RenderWindow& window, Event& event, const RenderStates& state)

#define handle_function(function) bool function(RenderWindow& window, const RenderStates& state = RenderStates::Default)
#define _handle_function(class_name,function) bool class_name::function(RenderWindow& window, const RenderStates& state)

#define HASHCODE "#C#D"
#endif