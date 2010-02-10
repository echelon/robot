#ifndef INTERNALS_REGISTRY_H
#define INTERNALS_REGISTRY_H

#include <map>

namespace Internals {

/**
 * The Registry class is part of the Robot Internals.
 * This allows us to track "global" variables.
 */
class Registry 
{
	public:
		/**
		 * Add an object to the registry.
		 * If an attempt is made to add to an existing key, there will be an error.
		 */
		static void add(const char* key, void* val);

		/**
		 * Set an object to a key in the registry.
		 * No error if overwriting.
		 */
		static void set(const char* key, void* val);

		/**
		 * Get an object from the registry.
		 */
		static void* get(const char* key);

	private:
		/**
		 * Singleton
		 */
		Registry() {}
		Registry(const Registry&);
		Registry& operator= (const Registry&);

		/**
		 * Map for the registry storage.
		 */
		static std::map<const char*, void*> registry;
};
}

#endif
