#include "dropitem.h"

void DropItem::toString(CharConv* cv)
{

	m_item->toString(cv);
	cv->toBuffer(m_x);
	cv->toBuffer(m_y);



}

void DropItem::fromString(CharConv* cv)
{
	m_item->fromString(cv);
	cv->fromBuffer<short>(m_x );
	cv->fromBuffer<short>(m_y);

}


int DropItem::getId()
{
	return 10000* m_x + m_y;
}

std::string DropItem::getNameId()
{
	std::ostringstream out_stream;

	out_stream.str("");
	out_stream << m_item->getName() << ":" << m_item->m_id;
	return out_stream.str();
}
