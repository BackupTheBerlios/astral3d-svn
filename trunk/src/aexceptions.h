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
 * @file aexceptions.h Exceptions.
 */
#ifndef AEXCEPTIONS_H
#define AEXCEPTIONS_H

#ifdef WIN32
    #include <windows.h>
#endif

#include <iostream>
#include <stdexcept>

/**
 * @namespace astral3d Astral3D namespace.
 */
namespace astral3d {

/**
 * Basic Astral3D Exception.
 */
class AException : public std::exception
{
    private:
        std::string message;

    public:
        /**
         * Constructor.
         * @param message Description of the exception
         */
        AException(const std::string &message)
        {
            this->message = message;
        }

        ~AException() throw() {}

        /**
         * Returns description of the exception.
         * @return Description of the exception
         */
        const char *what() const throw()
        {
            return message.c_str();
        }

        friend std::ostream& operator<<(std::ostream &out, const AException &ex)
        {
            out << ex.what();
            return out;
        }
};

/**
 * Null Pointer Exception. Inherits from AException class.
 */
class ANullPointerException : public AException
{
    public:
        /**
         * Constructor.
         * @param message Description of the exception
         */
        ANullPointerException(const std::string &message) : AException(message) {}
};

/**
 * Illegal Argument Exception. Inherits from AException class.
 */
class AIllegalArgumentException : public AException
{
    public:
        /**
         * Constructor.
         * @param message Description of the exception
         */
        AIllegalArgumentException(const std::string &message) : AException(message) {}
};

/**
 * Memory Alloc Exception. Inherits from AException class.
 */
class AMemoryAllocException : public AException
{
    public:
        /**
         * Constructor.
         * @param message Description of the exception
         */
        AMemoryAllocException(const std::string &message) : AException(message) {}
};

/**
 * Texture Exception. Inherits from AException class.
 */
class ATextureException : public AException
{
    public:
        /**
         * Constructor.
         * @param message Description of the exception
         */
        ATextureException(const std::string &message) : AException(message) {}
};

/**
 * IO Exception. Inherits from AException class.
 */
class AIOException : public AException
{
    public:
        /**
         * Constructor.
         * @param message Description of the exception
         */
        AIOException(const std::string &message) : AException(message) {}
};

/**
 * Read File Exception. Inherits from AIOException class.
 */
class AReadFileException : public AIOException
{
    public:
        /**
         * Constructor.
         * @param message Description of the exception
         */
        AReadFileException(const std::string &message) : AIOException(message) {}
};

/**
 *  Write File Exception. Inherits from AIOException class.
 */
class AWriteFileException : public AIOException
{
    public:
        /**
         * Constructor.
         * @param message Description of the exception
         */
        AWriteFileException(const std::string &message) : AIOException(message) {}
};

/**
 * SDL Exception. Inherits from AException class.
 */
class ASDLException : public AException
{
    public:
        /**
         * Constructor.
         * @param message Description of the exception
         */
        ASDLException(const std::string &message) : AException(message) {}
};

} // namespace astral3d

#endif    // #ifndef AEXCEPTIONS_H
