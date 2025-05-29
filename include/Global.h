#ifndef GLOBAL_H
#define GLOBAL_H

#ifdef _WIN32
typedef unsigned int uint;
typedef unsigned char uchar;
#endif

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
#include <glad/glad.h>
#include <glm/glm.hpp>

#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_transform.hpp"

#define catch_function(function) bool function(sf::RenderWindow& window, sf::Event& event, const sf::RenderStates& state = sf::RenderStates::Default)
#define _catch_function(class_name,function) bool class_name::function(sf::RenderWindow& window, sf::Event& event, const sf::RenderStates& state)

#define handle_function(function) bool function(sf::RenderWindow& window, const sf::RenderStates& state = sf::RenderStates::Default)
#define _handle_function(class_name,function) bool class_name::function(sf::RenderWindow& window, const sf::RenderStates& state)

#define HASHCODE "#C#D"
#endif