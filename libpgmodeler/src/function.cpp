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

#include "function.h"
#include "defaultlanguages.h"

Function::Function() : BaseFunction()
{
	return_type = PgSqlType(QString("void"));
	returns_setof=false;
	is_wnd_function=false;
	is_leakproof=false;
	obj_type=ObjectType::Function;

	execution_cost=100;
	row_amount=1000;

	attributes[Attributes::ExecutionCost]="";
	attributes[Attributes::RowAmount]="";
	attributes[Attributes::ReturnType]="";
	attributes[Attributes::FunctionType]="";
	attributes[Attributes::ReturnsSetOf]="";
	attributes[Attributes::BehaviorType]="";
	attributes[Attributes::RefType]="";
	attributes[Attributes::WindowFunc]="";
	attributes[Attributes::ReturnTable]="";
	attributes[Attributes::LeakProof]="";
}

void Function::addReturnedTableColumn(const QString &name, PgSqlType type)
{
	//Raises an error if the column name is empty
	if(name.isEmpty())
		throw Exception(ErrorCode::AsgEmptyNameTableReturnType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	vector<Parameter>::iterator itr,itr_end;
	bool found=false;

	itr=ret_table_columns.begin();
	itr_end=ret_table_columns.end();

	//Checks the duplicity of table column names
	while(itr!=itr_end && !found)
	{
		/* Compares the column name storing in the 'found' flag
		 if already exists in the returned table */
		found=(itr->getName()==name);
		itr++;
	}

	//Raises an error if the column is duplicated
	if(found)
		throw Exception(Exception::getErrorMessage(ErrorCode::InsDuplicatedTableReturnType)
						.arg(name)
						.arg(this->signature),
						ErrorCode::InsDuplicatedTableReturnType,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	Parameter p;
	p.setName(name);
	p.setType(type);
	ret_table_columns.push_back(p);
	setCodeInvalidated(true);
}

void Function::setTableReturnTypeAttribute(unsigned def_type)
{
	QString str_type;
	unsigned i, count;

	count=ret_table_columns.size();
	for(i=0; i < count; i++)
	{
		str_type+=ret_table_columns[i].getCodeDefinition(def_type);
	}

	if(def_type==SchemaParser::SqlDefinition)
		str_type.remove(str_type.size()-2,2);

	attributes[Attributes::ReturnTable]=str_type;
}

void Function::setExecutionCost(unsigned exec_cost)
{
	setCodeInvalidated(execution_cost != exec_cost);
	execution_cost=exec_cost;
}

void Function::setRowAmount(unsigned row_amount)
{
	setCodeInvalidated(this->row_amount != row_amount);
	this->row_amount=row_amount;
}

void Function::setReturnType(PgSqlType type)
{
	setCodeInvalidated(return_type != type);
	return_type=type;
	ret_table_columns.clear();
}

void Function::setFunctionType(FunctionType func_type)
{
	setCodeInvalidated(function_type != func_type);
	function_type=func_type;
}

void Function::setReturnSetOf(bool value)
{
	setCodeInvalidated(returns_setof != value);
	returns_setof = value;
	ret_table_columns.clear();
}

void Function::setWindowFunction(bool value)
{
	setCodeInvalidated(is_wnd_function != value);
	is_wnd_function=value;
}

void Function::setLeakProof(bool value)
{
	setCodeInvalidated(is_leakproof != value);
	is_leakproof=value;
}

void Function::setBehaviorType(BehaviorType behav_type)
{
	setCodeInvalidated(behavior_type != behav_type);
	behavior_type=behav_type;
}

PgSqlType Function::getReturnType()
{
	return return_type;
}

FunctionType Function::getFunctionType()
{
	return function_type;
}

unsigned Function::getReturnedTableColumnCount()
{
	return ret_table_columns.size();
}

bool Function::isReturnSetOf()
{
	return returns_setof;
}

bool Function::isReturnTable()
{
	return (ret_table_columns.size() > 0);
}

bool Function::isWindowFunction()
{
	return is_wnd_function;
}

bool Function::isLeakProof()
{
	return is_leakproof;
}

BehaviorType Function::getBehaviorType()
{
	return behavior_type;
}

Parameter Function::getReturnedTableColumn(unsigned column_idx)
{
	//Raises an error if the column index is out of bound
	if(column_idx>=ret_table_columns.size())
		throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	return ret_table_columns[column_idx];
}

unsigned Function::getExecutionCost()
{
	return execution_cost;
}

unsigned Function::getRowAmount()
{
	return row_amount;
}

void Function::removeReturnedTableColumns()
{
	ret_table_columns.clear();
	setCodeInvalidated(true);
}

void Function::removeReturnedTableColumn(unsigned column_idx)
{
	if(column_idx>=ret_table_columns.size())
		throw Exception(ErrorCode::RefObjectInvalidIndex,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	vector<Parameter>::iterator itr;
	itr=ret_table_columns.begin()+column_idx;
	ret_table_columns.erase(itr);
	setCodeInvalidated(true);
}

QString Function::getCodeDefinition(unsigned def_type)
{
	return this->getCodeDefinition(def_type, false);
}

QString Function::getCodeDefinition(unsigned def_type, bool reduced_form)
{
	QString code_def=getCachedCode(def_type, reduced_form);
	if(!code_def.isEmpty()) return code_def;

	setBasicFunctionAttributes(def_type);

	attributes[Attributes::ExecutionCost]=QString("%1").arg(execution_cost);
	attributes[Attributes::RowAmount]=QString("%1").arg(row_amount);
	attributes[Attributes::FunctionType]=(~function_type);

	if(def_type==SchemaParser::SqlDefinition)
		attributes[Attributes::ReturnType]=(*return_type);
	else
		attributes[Attributes::ReturnType]=return_type.getCodeDefinition(def_type);

	setTableReturnTypeAttribute(def_type);

	attributes[Attributes::ReturnsSetOf]=(returns_setof ? Attributes::True : "");
	attributes[Attributes::WindowFunc]=(is_wnd_function ? Attributes::True : "");
	attributes[Attributes::LeakProof]=(is_leakproof ? Attributes::True : "");
	attributes[Attributes::BehaviorType]=~behavior_type;

	return BaseObject::getCodeDefinition(def_type, reduced_form);
}

QString Function::getAlterDefinition(BaseObject *object)
{
	Function *func=dynamic_cast<Function *>(object);

	if(!func)
		throw Exception(ErrorCode::OprNotAllocatedObject,__PRETTY_FUNCTION__,__FILE__,__LINE__);

	try
	{
		attribs_map attribs;

		attributes[Attributes::AlterCmds]=BaseObject::getAlterDefinition(object);

		if(this->source_code.simplified() != func->source_code.simplified() ||
			 this->library!=func->library || this->symbol!=func->symbol)
		{
			attribs[Attributes::Definition]=func->getCodeDefinition(SchemaParser::SqlDefinition);
			attribs[Attributes::Definition].replace(QString("CREATE FUNCTION"), QString("CREATE OR REPLACE FUNCTION"));
		}
		else
		{
			if(this->execution_cost!=func->execution_cost)
				attribs[Attributes::ExecutionCost]=QString::number(func->execution_cost);

			if(this->returns_setof && func->returns_setof && this->row_amount!=func->row_amount)
			{
				attribs[Attributes::ReturnsSetOf]=Attributes::True;
				attribs[Attributes::RowAmount]=QString::number(row_amount);
			}

			if(this->function_type!=func->function_type)
				attribs[Attributes::FunctionType]=~func->function_type;

			if(this->is_leakproof!=func->is_leakproof)
				attribs[Attributes::LeakProof]=(func->is_leakproof ? Attributes::True : Attributes::Unset);

			if(this->security_type!=func->security_type)
				attribs[Attributes::SecurityType]=~func->security_type;

			if((this->behavior_type!=func->behavior_type) &&
					((this->behavior_type==BehaviorType::CalledOnNullInput) ||
					 ((this->behavior_type==BehaviorType::Strict || this->behavior_type==BehaviorType::ReturnsNullOnNullInput) &&
					  func->function_type==BehaviorType::CalledOnNullInput)))
				attribs[Attributes::BehaviorType]=~func->behavior_type;
		}

		copyAttributes(attribs);

		return BaseObject::getAlterDefinition(this->getSchemaName(), attributes, false, true);
	}
	catch(Exception &e)
	{
		throw Exception(e.getErrorMessage(),e.getErrorCode(),__PRETTY_FUNCTION__,__FILE__,__LINE__,&e);
	}
}

void Function::configureSearchAttributes()
{
	BaseFunction::configureSearchAttributes();
	search_attribs[Attributes::ReturnType] = ret_table_columns.empty() ? *return_type : "";
}
