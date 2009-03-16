#include "Registry.hpp"
#include <stdio.h>

namespace Internals {

std::map<const char*, void*> Registry::registry;

void Registry::add(const char* key, void* val)
{
	std::map<const char*, void*>::iterator iter = registry.find(key);
	if(iter != registry.end()) {
		// TODO: Some kind of error
		printf("Key already in registry!\n");
	}
	registry[key] = val;
}

void Registry::set(const char* key, void* val)
{
	registry[key] = val;
}

void* Registry::get(const char* key)
{
	std::map<const char*, void*>::iterator iter = registry.find(key);
	if(iter == registry.end()) {
		// TODO: Some kind of error.
		printf("Not in registry!\n");
	}
	return iter->second;
}


} // end namespace
