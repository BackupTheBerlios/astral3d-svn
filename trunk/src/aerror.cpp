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

#include "aerror.h"

using namespace std;
namespace astral3d {

// external Astral3D error implementation
AError aError;

//-----------------------------------------------------------------------------
// text output
//-----------------------------------------------------------------------------

ostream& operator<<(ostream& out, const AError &err)
{
    out << "Error Description: " << err.errorDescription << endl;
    out << "Sender Function: " << err.senderFunction << endl;
    out << "Failed Function: " << err.failedFunction << endl;

    return out;
}

//-----------------------------------------------------------------------------
// returns Astral3D error description
//-----------------------------------------------------------------------------

AError getAstral3DError()
{
    return aError;
}

//-----------------------------------------------------------------------------
// sets Astral3D error
//-----------------------------------------------------------------------------

void setAstral3DError(string description, string sender, string failed)
{
    aError.errorDescription = description;
    aError.senderFunction = sender;
    aError.failedFunction = failed;
}

} // namespace astral3d
