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
\ingroup libutils
\class Application
\brief This class inherits from QApplication and just sets up a search path in order to initialize GlobalAttribute static class
\note <strong>Creation date:</strong> 30/08/2007
*/

#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>
#include <QDir>

class Application: public QApplication {
	protected:
		/*! \brief Creates the pgModeler's configuration dir on user's local storage.
		 * The output path is platform dependant and is determined by GlobalAttributes::getConfigurationsDir() */
		void createUserConfiguration();

		//! \brief Copy files from a path to another recursively
		void copyFilesRecursively(const QString &src_path, const QString &dst_path);

	public:
		Application(int & argc, char ** argv);
};

#endif
