/*
# PostgreSQL Database Modeler (pgModeler)
#
# Copyright 2006-2020 - Raphael Araújo e Silva <raphael@pgmodeler.io>
#
# This program is free software: you can redistribute it and/or modify
# it under the terms of the GNU General Public License as published by
# the Free Software Foundation version 3.
#
# This program is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# The complete text of GPLv3 is at LICENSE file on source code root directory.
# Also, you can get the complete GNU General Public License at <http://www.gnu.org/licenses/>
*/

/**
\ingroup libpgmodeler
\class TypeArgument
\brief Implements the operations to manipulate user defined types arguments.
*/

#ifndef TYPE_ARGUMENT_H
#define TYPE_ARGUMENT_H

#include "baseobject.h"
#include "pgsqltypes/pgsqltype.h"

class TypeAttribute: public BaseObject {
	private:
		PgSqlType type;

		QString getCodeDefinition(unsigned, bool){ return ""; }

	public:
		TypeAttribute();
        TypeAttribute(const TypeAttribute &type_attr);

		void setType(PgSqlType type);
		PgSqlType getType();

		//! \brief Returns the SQL / XML code definition for the parameter
		virtual QString getCodeDefinition(unsigned def_type) final;
		void operator = (const TypeAttribute &tpattrib);
};

#endif
