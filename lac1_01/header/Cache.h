/*
 * Cache.h
 *
 *  Created on: Jul 25, 2012
 *      Author: itamar
 */

#ifndef CACHE_H_
#define CACHE_H_

typedef unsigned int ui;
typedef unsigned long int ul;


#include <set>
#include <unordered_map>
#include <map>
#include "FSBAllocator.hh"

using namespace std;

template<typename Key, typename Content> class Cache {
public:
	explicit Cache(ui pMaxSize)
	:maxSize(pMaxSize), nRemove(pMaxSize * 0.1 + 1), hits(0), misses(0){}
	void insert(const Key& key, const Content& content);
	bool get(const Key& key, Content &content);
	void clear(void);
	void printStatistics(void);
private:
	inline bool isFull(void) { return maxSize <= cacheContent.size();}
	void removeN(void);

	ui maxSize;
	ui nRemove;//percentage of the items to remove when the cache is full
	ul hits;
	ul misses;
	map<Key, Content> cacheContent;

};


template<typename Key, typename Content>
void Cache<Key, Content>::insert(const Key& key, const Content& content)
{
	if(cacheContent.find(key) == cacheContent.end()){
		if(isFull()){
			removeN();
		}
		cacheContent[key] = content;
	}
}

template<typename Key, typename Content>
bool Cache<Key, Content>::get(const Key& key, Content &content){
	typename map<Key, Content>::iterator it =  cacheContent.find(key);
	if(it != cacheContent.end()){
		++hits;
		content = (*it).second;
		return true;
	}

	++misses;
	return false;
}

template<typename Key, typename Content>
void Cache<Key, Content>::printStatistics(){
	printf("size %u hits %lu misses %lu usage %.2f\n", static_cast<ui>(cacheContent.size()),
			hits, misses, static_cast<float>(cacheContent.size())/static_cast<float>(maxSize));

}

template<typename Key, typename Content>
void Cache<Key, Content>::clear(){
	hits = 0;
	misses = 0;
	cacheContent.clear();
}

template<typename Key, typename Content>
void Cache<Key, Content>::removeN(void)
{
	typename map<Key, Content>::iterator it = cacheContent.end();
	for(ui i = 0; i < nRemove && it != cacheContent.begin(); ++i){
		--it;
	}
	cacheContent.erase(it, cacheContent.end());
}
//
//namespace std
//{
//    template<typename T>
//    struct hash<list<T> >
//    {
//        typedef list<T>  argument_type;
//        typedef size_t result_type;
//
//        result_type operator()(const argument_type& a) const
//        {
//            hash<T> hasher;
//            result_type h = 0;
//            for (const T&  i : a)
//            {
//            	h = h *31 +   hasher(i);
//            }
//            return h;
//        }
//    };
//
//    template<typename T>
//    struct hash<list<T, FSBAllocator<T> > >
//    {
//        typedef list<T, FSBAllocator<T> >  argument_type;
//        typedef size_t result_type;
//
//        result_type operator()(const argument_type& a) const
//        {
//            hash<T> hasher;
//            result_type h = 0;
//            for (const T&  i : a)
//            {
//            	h = h *31 +   hasher(i);
//            }
//            return h;
//        }
//    };
//}


#endif /* CACHE_H_ */
