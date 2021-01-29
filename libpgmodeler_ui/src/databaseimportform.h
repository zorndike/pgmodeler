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
\ingroup libpgmodeler_ui
\class DatabaseImportForm
\brief Implements the form to execute the reverse engineering operations
*/

#ifndef DATABASE_IMPORT_FORM_H
#define DATABASE_IMPORT_FORM_H

#include "ui_databaseimportform.h"
#include "databaseimporthelper.h"
#include "htmlitemdelegate.h"
#include "objectsfilterwidget.h"
#include <QTimer>

class DatabaseImportForm: public QDialog, public Ui::DatabaseImportForm {
	private:
		Q_OBJECT

		/*! \brief Indicates if the full output generated during the process should be displayed
		 * When this attribute is true, only errors and some key info messages are displayed. */
		static bool low_verbosity;
		
		//! \brief Custom delegate used to paint html texts in output tree
		HtmlItemDelegate *htmlitem_del;
		
		bool create_model;
			
		/*! \brief Model widget allocated during the import. In case of success this model
		will be transferred to the main window or destroyed in case of failure */
		ModelWidget *model_wgt;

		//! \brief Database importer helper
		DatabaseImportHelper *import_helper;
		
		//! \brief Thead that controls the database import helper
		QThread *import_thread;

		ObjectsFilterWidget *objs_filter_wgt;
		
		/*! \brief Toggles the checked state for the specified item. This method recursively
		changes the check state for the children items */
		void setItemCheckState(QTreeWidgetItem *item, Qt::CheckState chk_state);
		
		//! \brief Checks the item's parent when its checked
		void setParentItemChecked(QTreeWidgetItem *item);
		
		//! \brief Returns true when there is at least one item checked on the objects tree
		bool hasObjectsToImport();
		
		/*! \brief Returns the items oids in "obj_oids" map. The second parameter
		"col_oids" stores the columns oids for each selected table */
		void getObjectToImport(map<ObjectType, vector<unsigned>> &obj_oids, map<unsigned, vector<unsigned>> &col_oids);
		
		void finishImport(const QString &msg);
		void showEvent(QShowEvent *);
		void closeEvent(QCloseEvent *event);
		void destroyModelWidget();
		
		//! \brief Allocates the import thread and helper
		void createThread();
		
		//! \brief Destroys both import thread and helper
		void destroyThread();

		//! \brief Filters and list in a different way the objects that matches the user provided filters
		void listFilteredObjects();

	public:
		//! \brief Constants used to access the tree widget items data
		static constexpr unsigned ObjectId=1,
		ObjectTypeId=2,
		ObjectName=3,
		ObjectSchema=4,
		ObjectTable=5,
		ObjectAttribs=6, //Stores the object's attributes returned by catalog query
		ObjectOtherData=7, //General purpose usage
		ObjectCount=8,
		ObjectSource=9; //Only for gropus

		/*! \brief This constant holds the maximum amount of objects in a database to be imported
		 * which will not generate an alert message about the possible slowdowns in the process
		 * if all objects are imported without using filters */
		static constexpr unsigned ObjectCountThreshould=2000;
		
		DatabaseImportForm(QWidget * parent = nullptr, Qt::WindowFlags f = Qt::Widget);
		virtual ~DatabaseImportForm();
		
		void setModelWidget(ModelWidget *model);

		//! \brief Defines if all the output generated during the import process should be displayed
		static void setLowVerbosity(bool value);
		
		//! \brief Returns the configured model widget
		ModelWidget *getModelWidget();
		
		//! \brief Fills a combo box with all available databases according to the configurations of the specified import helper
		static void listDatabases(DatabaseImportHelper &import_helper, QComboBox *dbcombo);
		
		/*! \brief Fills a tree widget with all available database objects according to the configurations of the specified import helper.
		The parameter 'disable_empty_grps' will make empty group items disabled. The parameter 'create_db_item' will create the root
		item representing the database itself. The parameter 'create_dummy_item' create an empty child item that represent schema or table
		child. In this case the generation of schema's or table's children need to be done manually. */
		static void listObjects(DatabaseImportHelper &import_helper, QTreeWidget *tree_wgt, bool checkable_items,
														bool disable_empty_grps, bool create_db_item, bool create_dummy_item = false,
														int sort_by = 0);

		/*! \brief Fills a table widget by searching only objects matching the filters configured in the provided import helper
		 * This method will force the first item of each row to be checkable also it'll adjust the column count to fit all info retrieved from catalog */
		static void listFilteredObjects(DatabaseImportHelper &import_hlp, QTableWidget *flt_objects_tbw);
		
		/*! \brief Filters an tree widget using a pattern. The 'search_column' indicates in which column the pattern is applied.
		The paramenter 'sel_single_leaf' indicates if the single leaf (resulting from filtering) must be selected. */
		static void filterObjects(QTreeWidget *db_objects_tw, const QString &pattern, int search_column, bool sel_single_leaf);
		
		/*! \brief Retrieve the specified objects from the database and insert them onto the tree view.
		The "root" parameter is used to associate the group of objects as child of it.
		The "schema" and "table" parameter are used to filter objects by schema and/or table.
		This method automatically returns a list of QTreeWidgetItem when the vector "types" contains ObjectType::ObjSchema or ObjectType::Table or ObjectType::View,
		The sort_by param indicates the column index in which the tree should be sorted by. When the sort_by is negative not sorting will be performed.*/
		static vector<QTreeWidgetItem *> updateObjectsTree(DatabaseImportHelper &import_helper, QTreeWidget *tree_wgt, vector<ObjectType> types,
																											 bool checkable_items=false, bool disable_empty_grps=true, QTreeWidgetItem *root=nullptr,
																											 const QString &schema="", const QString &table="");

	private slots:
		void importDatabase();
		void listObjects();
		void listDatabases();
		void updateProgress(int progress, QString msg, ObjectType obj_type);
		void cancelImport();
		void handleImportCanceled();
		void handleImportFinished(Exception e);
		void captureThreadError(Exception e);
		void filterObjects();
		
		//! \brief Toggles the check state for the specified item
		void setItemCheckState(QTreeWidgetItem *item,int);
		
		//! \brief Toggles the check state for all items
		void setItemsCheckState();
		
	signals:
		/*! \brief This signal is emitted whenever the user changes the connections settings
		within this widget without use the main configurations dialog */
		void s_connectionsUpdateRequest();
};

#endif
