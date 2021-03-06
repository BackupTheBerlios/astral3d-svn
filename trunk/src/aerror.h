 /*****************************************************************************
 * Astral3D -- 3D engine based on OpenGL and SDL.                             *
 * Copyright (C) 2005 Pavel Stupka <pavel.stupka@gmail.com>                   *
 *                                                                            *
 * This library is free software; you can redistribute it and/or              *
 * modify it under the terms of the GNU Lesser General Public                 *
 * License as published by the Free Software Foundation; either               *
 * version 2.1 of the License, or (at your option) any later version.         *
 *                                                                            *
 * This library is distributed in the hope that it will be useful,            *
 * but WITHOUT ANY WARRANTY; without even the implied warranty of             *
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU          *
 * Lesser General Public License for more details.                            *
 *                                                                            *
 * You should have received a copy of the GNU Lesser General Public           *
 * License along with this library; if not, write to the Free Software        *
 * Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA *
 *****************************************************************************/

/**
 * @file aerror.h AError class.
 */
#ifndef AERROR_H
#define AERROR_H

#ifdef WIN32
    #include <windows.h>
#endif

#include <iostream>
#include <string>
#include <sstream>

/**
 * @namespace astral3d Astral3D namespace.
 */
namespace astral3d {

/**
 * Structure containing error description.
 */
struct AError
{
    std::string errorDescription;     // description of the error
    std::string senderFunction;       // function that send this error
    std::string failedFunction;       // function that failed

    friend std::ostream& operator<<(std::ostream&, const AError&);
};

/**
 * Returns description of the last Astral3D error.
 * @return AError structure containing the error specification
 */
AError getAstral3DError();

/**
 * Sets Astral3D error.
 * This function sets the Astral3D error description. It is used by Atral3D
 * classes and functions and should not be called by user.
 * @param description Error description
 * @param sender Function that generates this error
 * @param failed Function that failed
 */
void setAstral3DError(std::string description, std::string sender, std::string failed);

} // namespace astral3d

#endif    // #ifndef AERROR_H
