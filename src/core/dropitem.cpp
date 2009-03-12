#include "dropitem.h"

DropItem::DropItem()
{
	m_angle_z = Random::random()*3.14159*2;

	float hadd = 0.1;
	float acc = -20;
	float v0 =4;
	m_height = 2.0 +hadd;
	
	
	float t = -v0/(2*acc)+sqrt(v0*v0/(4*acc*acc) - 2/acc);
	m_speed = 4/t;
	m_angle_x = 4;
	
	m_time =0;
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
	float hadd = 0.1;
			
	if (m_height>hadd)
	{
		float acc = -20;
		float v0 =4;
		m_height =hadd+ 2 + v0*m_time/1000 + acc*m_time*m_time/1000000;
		m_angle_x -= m_speed*time/1000;
		
		if (m_height < hadd)
		{
			m_height =hadd;
			m_angle_x=0;
		}
		DEBUG5("speed %f height %f angle %f time %f",m_speed,m_height,m_angle_x,m_time); 
	}
	
}

