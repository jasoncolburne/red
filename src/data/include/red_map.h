/* Copyright (C) 2010-2022, Jason Colburne
 *
 * Not to be used without express written consent.
 * All rights reserved.
 *
 * $Id: he_map.h 18 2010-04-17 05:57:03Z jason $
 */

#ifndef __RED_MAP_H__
#define __RED_MAP_H__


/*
 * list, set, hashtable
 *
 * redCollectionForEach( collection, function )
 * redCollectionFilter( collection, predicate )
 * redListSort( list, function )
 * redListFoldL( list, function, object)
 *   ( [1, 2, 3], mult, 2 )    -> (((2 * 1) * 2) * 3)
 *   ( [1, 2, 3], mult, null ) -> (((1) * 2) * 3)
 * redListFoldR( list, function, object)
 *   ( [1, 2, 3], mult, 2 )    -> (1 * (2 * (3 * 2)))
 *   ( [1, 2, 3], mult, null ) -> (1 * (2 * (3)))
 * redCollectionTransform( collection, mapping )
 *
 */

#endif /* __RED_MAP_H__ */
