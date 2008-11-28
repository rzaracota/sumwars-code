#ifndef EVENT_H
#define EVENT_H

#include <list>

/**
 * \fn class LogicalExpr
 * \brief Klasse fuer einen logischen Ausdruck
 */
class LogicalExpr
{
	public:
		/**
		* \fn virtual ~LogicalExpr() =0
		* \brief Destruktor
		*/
		virtual ~LogicalExpr() =0;
		
		/**
		* \fn virtual bool eval() =0
		* \brief wertet den Ausdruck aus
		*/
		virtual bool eval() =0;
	
};

/**
 * \fn class LogicalNot
 * \brief Klasse fuer Logisches Nicht
 */
class LogicalNot : public LogicalExpr
{
	public:
		
		/**
		 * \fn LogicalNot(LogicalExpr* expr=0)
	 	 * \brief Konstruktor
		 * \param expr Ausdruck der negiert wird
		 */
		LogicalNot(LogicalExpr* expr=0)
		{
			m_expr = expr;
		}
		
		/**
		 * \fn void setExpression(LogicalExpr* expr)
		 * \brief Setzt den Ausdruck der negiert wird
		 * \param expr Ausdruck der negiert wird
		 */
		void setExpression(LogicalExpr* expr)
		{
			m_expr = expr;
		}
		
		/**
		* \fn virtual ~LogicalNot() 
		* \brief Destruktor
		*/
		virtual ~LogicalNot()
		{
			delete m_expr;
		}
			
		/**
		* \fn virtual bool eval()
		* \brief wertet den Ausdruck aus
		*/
		virtual bool eval()
		{
			return  !(m_expr->eval());
		}
		
	private:
		/**
		/* \var LogicalExpr* m_expr
		 * \brief Ausdruck der negiert wird
		 */
		LogicalExpr* m_expr;
};


/**
 * \class LogicalAnd
 * \brief Klasse fuer logisches Und
 */
class LogicalAnd : public LogicalExpr
{
	public:
		/**
		 * \fn virtual ~LogicalAnd()
		 * \brief Destruktor
		 */
		virtual ~LogicalAnd();
		
		/**
		 * \fn void addExpression(LogicalExpr* expr)
		 * \brief Fuegt der Undverknuepfung einen Ausdruck hinzu
		 */
		void addExpression(LogicalExpr* expr)
		{
			m_expressions.push_back(expr);
		}
		
		/**
		 * \fn virtual bool eval()
		 * \brief wertet den Ausdruck aus
		 */
		virtual bool eval();
	
	private:
		/**
		 * \var std::list<LogicalExpr*> m_expressions
		 * \brief Ausdruecke, die mit and verknuepft werden
		 */
		std::list<LogicalExpr*> m_expressions;
};

/**
 * \class LogicalOr
 * \brief Klasse fuer logisches Oder
 */
class LogicalOr : public LogicalExpr
{
	public:
		/**
		 * \fn virtual ~LogicalOr()
		 * \brief Destruktor
		 */
		virtual ~LogicalOr();
		
		/**
		 * \fn void addExpression(LogicalExpr* expr)
		 * \brief Fuegt der Oderverknuepfung einen Ausdruck hinzu
		 */
		void addExpression(LogicalExpr* expr)
		{
			m_expressions.push_back(expr);
		}
		
		/**
		 * \fn virtual bool eval()
		 * \brief wertet den Ausdruck aus
		 */
		virtual bool eval();
	
	private:
		/**
		 * \var std::list<LogicalExpr*> m_expressions
		 * \brief Ausdruecke, die mit oder verknuepft werden
		 */
		std::list<LogicalExpr*> m_expressions;
};



#endif