#include "dropitem.h"

DropItem::DropItem()
{
	m_angle_z = Random::random()*3.14159*2;

	m_height = 2.0;
	m_speed = -0.1;
	
	float t = -m_speed/0.001 + sqrt(m_speed*m_speed/(0.001*0.001) + m_height*2/0.001);
	m_angle_x = t/50;
	
}

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

void DropItem::update(float time)
{
	m_time += time;
	
	if (m_height>0)
	{
		m_speed += time/1000;
		m_angle_x -= time/50;
		m_height -= m_speed;
		
		if (m_height < 0)
		{
			m_height =0;
			m_angle_x=0;
		}
	}
	
}

