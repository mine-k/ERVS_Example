#include "EyedeaInterface.h"
#include "EyedeaErrorType.h"

CEyedeaInterface::CEyedeaInterface(void) :
	m_cls_eth_client(NULL)
{
#if 0
	//start
	m_run_thread_net = true;

	m_thread_net = boost::thread(boost::bind(&CEyedeaInterface::ThreadFunctionNetwork, this));
#endif
}

CEyedeaInterface::~CEyedeaInterface(void)
{
	if(m_cls_eth_client)
	{
		m_cls_eth_client->Close();

		delete m_cls_eth_client;
		m_cls_eth_client = NULL;
	}

	Close();
}

void CEyedeaInterface::Close(void)
{
#if 0
	m_run_thread_net = false;		//thread close

	m_thread_net.join();			//wait
#endif
}

#if 0
int CEyedeaInterface::Connect(char* ip, char* port)
{
	if (m_cls_eth_client)
	{
		m_cls_eth_client->Close();

		delete m_cls_eth_client;
		m_cls_eth_client = NULL;
	}

	m_cls_eth_client = new CEyedeaEthernetClient(ip, port);
}
#endif

int CEyedeaInterface::SetVisionConfigOption(int option, float value)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_VISION_CONFIG_SET;

	int len = 8;
	unsigned char* data = new unsigned char[len];
	
	//option
	data[0] = (option & 0xFF000000) >> 24;
	data[1] = (option & 0x00FF0000) >> 16;
	data[2] = (option & 0x0000FF00) >> 8;
	data[3] = (option & 0x000000FF);

	//x
	int i_value = value * 1000.0;
	data[4] = (i_value & 0xFF000000) >> 24;
	data[5] = (i_value & 0x00FF0000) >> 16;
	data[6] = (i_value & 0x0000FF00) >> 8;
	data[7] = (i_value & 0x000000FF);

	unsigned int scale_factor = 1000;
	int ret = 0;
    ret = m_cls_eth_client->Send(command, &scale_factor, (unsigned char **)&data, &len);
	if(ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if(ret == 0){
				ret = m_cls_eth_client->Send(command, &scale_factor, (unsigned char **)&data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

float CEyedeaInterface::GetVisionConfigOption(int option)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_VISION_CONFIG_GET;

	int len = 4;
	unsigned char* data = new unsigned char[len];

	//option
	data[0] = (option & 0xFF000000) >> 24;
	data[1] = (option & 0x00FF0000) >> 16;
	data[2] = (option & 0x0000FF00) >> 8;
	data[3] = (option & 0x000000FF);

	unsigned int scale_factor = 1;
	float ret = 0;
    ret = m_cls_eth_client->Send(command, &scale_factor, (unsigned char **)&data, &len);
	if(ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if(ret == 0){
				ret = m_cls_eth_client->Send(command, &scale_factor, (unsigned char **)&data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}
		return ret;
	}

	ret = 0.0;

	if (len >= 4)
	{
		//x
		int i_value = ((int)data[0] << 24) & 0xFF000000;
		i_value |= ((int)data[1] << 16) & 0x00FF0000;
		i_value |= ((int)data[2] << 8) & 0x0000FF00;
		i_value |= ((int)data[3]) & 0x000000FF;

		ret =(float)i_value / (float)scale_factor;
	}

	delete data;
	data = NULL;

	return ret;
}

int CEyedeaInterface::DBAddObject(void)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	//printf("SetSearchArea - %d %d %d %d\n", x, y, w, h);

	char command = COMMAND_DB_ADD_OBJECT;
	int len = 0;
	unsigned char* data = NULL;

	unsigned int scale_factor = 1;
	int new_id = 0;
    new_id = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if(new_id == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			new_id = m_cls_eth_client->Open(m_ip, m_port);
			if(new_id == 0){
				new_id = m_cls_eth_client->Send(command, &scale_factor, (unsigned char **)&data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}

					return new_id;
				}
				continue;
			}
		}
	}
	if (new_id != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}
		return new_id;
	}

	new_id = -1;

	if (len >= 4)
	{
		new_id = ((int)data[0] << 24) & 0xFF000000;
		new_id |= ((int)data[1] << 16) & 0x00FF0000;
		new_id |= ((int)data[2] << 8) & 0x0000FF00;
		new_id |= ((int)data[3]) & 0x000000FF;
	}

	if (data != NULL)
	{
		delete (data);
		data = NULL;
	}

	return new_id;
}

int CEyedeaInterface::DB_Del_ID(int id)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	//printf("SetSearchArea - %d %d %d %d\n", x, y, w, h);

	char command = COMMAND_DB_DEL_ID;
	int len = 4;
	unsigned char* data = new unsigned char[len];

	//option
	data[0] = (id & 0xFF000000) >> 24;
	data[1] = (id & 0x00FF0000) >> 16;
	data[2] = (id & 0x0000FF00) >> 8;
	data[3] = (id & 0x000000FF);

	unsigned int scale_factor = 1;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}
		return ret;
	}

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::GetDB_Select_ID(void)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	//printf("SetSearchArea - %d %d %d %d\n", x, y, w, h);

	char command = COMMAND_DB_GET_SELECT_ID;
	int len = 0;
	unsigned char* data = NULL;

	unsigned int scale_factor = 1;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}
		return ret;
	}

	int ret_id = 0;
	if (len >= 4)
	{
		ret_id = ((int)data[0] << 24) & 0xFF000000;
		ret_id |= ((int)data[1] << 16) & 0x00FF0000;
		ret_id |= ((int)data[2] << 8) & 0x0000FF00;
		ret_id |= ((int)data[3]) & 0x000000FF;
	}

	ret = ret_id;

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::GetDB_Info_Id(int index)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	//printf("SetSearchArea - %d %d %d %d\n", x, y, w, h);

	char command = COMMAND_DB_GET_ID;
	int len = 4;
	unsigned char* data = new unsigned char[len];

	//option
	data[0] = (index & 0xFF000000) >> 24;
	data[1] = (index & 0x00FF0000) >> 16;
	data[2] = (index & 0x0000FF00) >> 8;
	data[3] = (index & 0x000000FF);

	unsigned int scale_factor = 1;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}
		return ret;
	}

	int ret_id = 0;
	if (len >= 4)
	{
		ret_id = ((int)data[0] << 24) & 0xFF000000;
		ret_id |= ((int)data[1] << 16) & 0x00FF0000;
		ret_id |= ((int)data[2] << 8) & 0x0000FF00;
		ret_id |= ((int)data[3]) & 0x000000FF;
	}

	ret = ret_id;

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::GetDBCount(void)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	//printf("SetSearchArea - %d %d %d %d\n", x, y, w, h);

	char command = COMMAND_DB_GET_COUNT;
	int len = 0;
	unsigned char* data = NULL ;

	unsigned int scale_factor = 1;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}
		return ret;
	}

	int count = 0;
	if (len >= 4)
	{
		count = ((int)data[0] << 24) & 0xFF000000;
		count |= ((int)data[1] << 16) & 0x00FF0000;
		count |= ((int)data[2] << 8) & 0x0000FF00;
		count |= ((int)data[3]) & 0x000000FF;
	}

	ret = count;

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::SetObject(int id)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	//printf("SetSearchArea - %d %d %d %d\n", x, y, w, h);

	char command = COMMAND_SET_OBJECT;
	int len = 4;
	unsigned char* data = new unsigned char[len];

	//option
	data[0] = (id & 0xFF000000) >> 24;
	data[1] = (id & 0x00FF0000) >> 16;
	data[2] = (id & 0x0000FF00) >> 8;
	data[3] = (id & 0x000000FF);

	unsigned int scale_factor = 1;
	int ret = 0;
    ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if(ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if(ret == 0){
				ret = m_cls_eth_client->Send(command, &scale_factor, (unsigned char **)&data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}
		return ret;
	}

	int ret_id = 0;
	if (len >= 4)
	{
		ret_id = ((int)data[0] << 24) & 0xFF000000;
		ret_id |= ((int)data[1] << 16) & 0x00FF0000;
		ret_id |= ((int)data[2] << 8) & 0x0000FF00;
		ret_id |= ((int)data[3]) & 0x000000FF;
	}

	ret = ret_id;

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

    return ret;
}

int CEyedeaInterface::GetImage(int option, int option2, char** out_data, int* len)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GET_IMAGE_W_OPTION;
	
	//option
	(*out_data)[0] = (option & 0xFF000000) >> 24;
	(*out_data)[1] = (option & 0x00FF0000) >> 16;
	(*out_data)[2] = (option & 0x0000FF00) >> 8;
	(*out_data)[3] = (option & 0x000000FF);

	//option
	(*out_data)[4] = (option2 & 0xFF000000) >> 24;
	(*out_data)[5] = (option2 & 0x00FF0000) >> 16;
	(*out_data)[6] = (option2 & 0x0000FF00) >> 8;
	(*out_data)[7] = (option2 & 0x000000FF);

	unsigned int scale_factor = 1;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, (unsigned char **)out_data, len);
	if(ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if(ret == 0){
				ret = m_cls_eth_client->Send(command, &scale_factor, (unsigned char **)out_data, len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if(sec >= 60)
					return ret;
				continue;
			}
		}
	}
	return ret;
}

int CEyedeaInterface::GetResultImage(int option, int option2, char** out_data, int* len)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GET_RESULT_IMAGE_FIND_OBJECTS;

	//option
	(*out_data)[0] = (option & 0xFF000000) >> 24;
	(*out_data)[1] = (option & 0x00FF0000) >> 16;
	(*out_data)[2] = (option & 0x0000FF00) >> 8;
	(*out_data)[3] = (option & 0x000000FF);

	//option
	(*out_data)[4] = (option2 & 0xFF000000) >> 24;
	(*out_data)[5] = (option2 & 0x00FF0000) >> 16;
	(*out_data)[6] = (option2 & 0x0000FF00) >> 8;
	(*out_data)[7] = (option2 & 0x000000FF);

	unsigned int scale_factor = 1;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, (unsigned char **)out_data, len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, (unsigned char **)out_data, len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
					return ret;
				continue;
			}
		}
	}
	return ret;
}

int CEyedeaInterface::SetSelectBaseObject(float x, float y, float w, float h)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	//printf("SetMasterArea - %d %d %d %d\n", x, y, w, h);

	char command = COMMAND_SELECT_BASE_OBJECT;
	unsigned char* data = new unsigned char[16];
	int len = 16;

	int _x = x * 10000;
	int _y = y * 10000;
	int _w = w * 10000;
	int _h = h * 10000;

	//x
	data[0] = (_x & 0xFF000000) >> 24;
	data[1] = (_x & 0x00FF0000) >> 16;
	data[2] = (_x & 0x0000FF00) >> 8;
	data[3] = (_x & 0x000000FF);

	//y
	data[4] = (_y & 0xFF000000) >> 24;
	data[5] = (_y & 0x00FF0000) >> 16;
	data[6] = (_y & 0x0000FF00) >> 8;
	data[7] = (_y & 0x000000FF);

	//w
	data[8] = (_w & 0xFF000000) >> 24;
	data[9] = (_w & 0x00FF0000) >> 16;
	data[10] = (_w & 0x0000FF00) >> 8;
	data[11] = (_w & 0x000000FF);

	//h
	data[12] = (_h & 0xFF000000) >> 24;
	data[13] = (_h & 0x00FF0000) >> 16;
	data[14] = (_h & 0x0000FF00) >> 8;
	data[15] = (_h & 0x000000FF);

	unsigned int scale_factor = 10000;
	int ret = 0;
    ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if(ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if(ret == 0){
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

    return ret;
}

int CEyedeaInterface::SetZoomArea(float x, float y, float w, float h)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	//printf("SetMasterArea - %d %d %d %d\n", x, y, w, h);

	char command = COMMAND_SET_ZOOM_AREA;
	unsigned char* data = new unsigned char[16];
	int len = 16;

	int _x = x * 10000;
	int _y = y * 10000;
	int _w = w * 10000;
	int _h = h * 10000;

	//x
	data[0] = (_x & 0xFF000000) >> 24;
	data[1] = (_x & 0x00FF0000) >> 16;
	data[2] = (_x & 0x0000FF00) >> 8;
	data[3] = (_x & 0x000000FF);

	//y
	data[4] = (_y & 0xFF000000) >> 24;
	data[5] = (_y & 0x00FF0000) >> 16;
	data[6] = (_y & 0x0000FF00) >> 8;
	data[7] = (_y & 0x000000FF);

	//w
	data[8] = (_w & 0xFF000000) >> 24;
	data[9] = (_w & 0x00FF0000) >> 16;
	data[10] = (_w & 0x0000FF00) >> 8;
	data[11] = (_w & 0x000000FF);

	//h
	data[12] = (_h & 0xFF000000) >> 24;
	data[13] = (_h & 0x00FF0000) >> 16;
	data[14] = (_h & 0x0000FF00) >> 8;
	data[15] = (_h & 0x000000FF);

	unsigned int scale_factor = 10000;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::SetMaskArea(float x, float y, float w, float h, bool inverse)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	//printf("SetMasterArea - %d %d %d %d\n", x, y, w, h);

	char command = COMMAND_SET_MASK_AREA;
	int len = 20;
	unsigned char* data = new unsigned char[len];

	int _x = x * 10000;
	int _y = y * 10000;
	int _w = w * 10000;
	int _h = h * 10000;
	int i = 0;
	if (inverse) i = 1;
	int _i = i * 10000;

	//x
	data[0] = (_x & 0xFF000000) >> 24;
	data[1] = (_x & 0x00FF0000) >> 16;
	data[2] = (_x & 0x0000FF00) >> 8;
	data[3] = (_x & 0x000000FF);

	//y
	data[4] = (_y & 0xFF000000) >> 24;
	data[5] = (_y & 0x00FF0000) >> 16;
	data[6] = (_y & 0x0000FF00) >> 8;
	data[7] = (_y & 0x000000FF);

	//w
	data[8] = (_w & 0xFF000000) >> 24;
	data[9] = (_w & 0x00FF0000) >> 16;
	data[10] = (_w & 0x0000FF00) >> 8;
	data[11] = (_w & 0x000000FF);

	//h
	data[12] = (_h & 0xFF000000) >> 24;
	data[13] = (_h & 0x00FF0000) >> 16;
	data[14] = (_h & 0x0000FF00) >> 8;
	data[15] = (_h & 0x000000FF);

	//i
	data[16] = (_i & 0xFF000000) >> 24;
	data[17] = (_i & 0x00FF0000) >> 16;
	data[18] = (_i & 0x0000FF00) >> 8;
	data[19] = (_i & 0x000000FF);

	unsigned int scale_factor = 10000;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::UndoMaskArea(void)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	//printf("SetMasterArea - %d %d %d %d\n", x, y, w, h);

	char command = COMMAND_UNDO_MASK_AREA;
	int len = 0;
	unsigned char* data = NULL;

	unsigned int scale_factor = 10000;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::DelMaskArea(void)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	//printf("SetMasterArea - %d %d %d %d\n", x, y, w, h);

	char command = COMMAND_DEL_MASK_AREA;
	int len = 0;
	unsigned char* data = NULL;

	unsigned int scale_factor = 10000;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::ResetZoomArea(void)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	//printf("SetMasterArea - %d %d %d %d\n", x, y, w, h);

	char command = COMMAND_RESET_ZOOM_AREA;
	unsigned char* data = NULL;
	int len = 0;

	unsigned int scale_factor = 10000;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::GetObjectCircle(float *out_x, float *out_y, float *out_r1, float *out_r2)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GET_OBJECT;
	int len = 4;
	unsigned char* data = new unsigned char[len];

	int type = 1;		//circle

	//x
	data[0] = (type & 0xFF000000) >> 24;
	data[1] = (type & 0x00FF0000) >> 16;
	data[2] = (type & 0x0000FF00) >> 8;
	data[3] = (type & 0x000000FF);

	unsigned int scale_factor = 1;
	int ret = 0;
    ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if(ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if(ret == 0){
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}

					return ret;
				}
				continue;
			}
		}
	}
	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}
		return ret;
	}

	if (len >= 16)
	{
		//x
		int x = ((int)data[0] << 24) & 0xFF000000;
		x |= ((int)data[1] << 16) & 0x00FF0000;
		x |= ((int)data[2] << 8) & 0x0000FF00;
		x |= ((int)data[3]) & 0x000000FF;

		//y
		int y = ((int)data[4] << 24) & 0xFF000000;
		y |= ((int)data[5] << 16) & 0x00FF0000;
		y |= ((int)data[6] << 8) & 0x0000FF00;
		y |= ((int)data[7]) & 0x000000FF;

		//r1
		int r1 = ((int)data[8] << 24) & 0xFF000000;
		r1 |= ((int)data[9] << 16) & 0x00FF0000;
		r1 |= ((int)data[10] << 8) & 0x0000FF00;
		r1 |= ((int)data[11]) & 0x000000FF;

		//r2
		int r2 = ((int)data[12] << 24) & 0xFF000000;
		r2 |= ((int)data[13] << 16) & 0x00FF0000;
		r2 |= ((int)data[14] << 8) & 0x0000FF00;
		r2 |= ((int)data[15]) & 0x000000FF;

		float _x = (float)x / (float)scale_factor;
		//_x = (float)(((_x*640.0) * (float)TRANS_IAMGE_WIDTH) / 640.0);
		//x = (int)((float)m_source.gray.cols*((_x*640.0))/((float)m_source.gray.cols)) ;
		//x = (int)((float)m_source.gray.cols * _x) ;

		float _y = (float)y / (float)scale_factor;
		//_y = (float)(((_y*480.0) * (float)TRANS_IAMGE_HEIGHT) / 480.0);
		//y = (int)((float)m_source.gray.rows*((_y*480.0)/((float)m_source.gray.rows))) ;
		//y = (int)((float)m_source.gray.rows * _y) ;

		float _r1 = (float)r1 / (float)scale_factor;
		//_r1 = (float)(((_r1*640.0) * (float)TRANS_IAMGE_WIDTH) / 640.0);
		//w = (int)((float)m_source.gray.cols*((_w*640.0))/((float)m_source.gray.cols)) ;
		//w = (int)((float)m_source.gray.cols * _w) ;

		float _r2 = (float)r2 / (float)scale_factor;
		//_r2 = (float)(((_r2*480.0) * (float)TRANS_IAMGE_HEIGHT) / 480.0);
		//h = (int)((float)m_source.gray.rows*((_h*480.0)/((float)m_source.gray.rows))) ;
		//h = (int)((float)m_source.gray.rows * _h) ;

		(*out_x) = _x ;
		(*out_y) = _y ;
		(*out_r1) = _r1 ;
		(*out_r2) = _r2 ;
	}

	delete (data);
	data = NULL;

    return ret;
}

int CEyedeaInterface::SetObjectCircle(float x, float y, float r1, float r2)
{
	//printf("test = %f, %f, %f, %f\n", x, y, r1, r2);

	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_SET_OBJECT_CIRCLE;
	unsigned char* data = new unsigned char[16];
	int len = 16;

	int _x = x*10000;
	int _y = y*10000;
	int _r1 = r1*10000;
	int _r2 = r2*10000;
	

	//x
	data[0] = (_x & 0xFF000000) >> 24;
	data[1] = (_x & 0x00FF0000) >> 16;
	data[2] = (_x & 0x0000FF00) >> 8;
	data[3] = (_x & 0x000000FF);

	//y
	data[4] = (_y & 0xFF000000) >> 24;
	data[5] = (_y & 0x00FF0000) >> 16;
	data[6] = (_y & 0x0000FF00) >> 8;
	data[7] = (_y & 0x000000FF);

	//r1
	data[8] = (_r1 & 0xFF000000) >> 24;
	data[9] = (_r1 & 0x00FF0000) >> 16;
	data[10] = (_r1 & 0x0000FF00) >> 8;
	data[11] = (_r1 & 0x000000FF);

	//r2
	data[12] = (_r2 & 0xFF000000) >> 24;
	data[13] = (_r2 & 0x00FF0000) >> 16;
	data[14] = (_r2 & 0x0000FF00) >> 8;
	data[15] = (_r2 & 0x000000FF);


	unsigned int scale_factor = 10000;
	int ret = 0;
    ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if(ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if(ret == 0){
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

    return ret;
}

int CEyedeaInterface::SetObjectLine(float x, float y, float w, float h)
{
	//printf("test = %f, %f, %f, %f\n", x, y, r1, r2);

	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_SET_OBJECT_LINE;
	unsigned char* data = new unsigned char[16];
	int len = 16;

	int _x = x * 10000;
	int _y = y * 10000;
	int _w = w * 10000;
	int _h = h * 10000;


	//x
	data[0] = (_x & 0xFF000000) >> 24;
	data[1] = (_x & 0x00FF0000) >> 16;
	data[2] = (_x & 0x0000FF00) >> 8;
	data[3] = (_x & 0x000000FF);

	//y
	data[4] = (_y & 0xFF000000) >> 24;
	data[5] = (_y & 0x00FF0000) >> 16;
	data[6] = (_y & 0x0000FF00) >> 8;
	data[7] = (_y & 0x000000FF);

	//r1
	data[8] = (_w & 0xFF000000) >> 24;
	data[9] = (_w & 0x00FF0000) >> 16;
	data[10] = (_w & 0x0000FF00) >> 8;
	data[11] = (_w & 0x000000FF);

	//r2
	data[12] = (_h & 0xFF000000) >> 24;
	data[13] = (_h & 0x00FF0000) >> 16;
	data[14] = (_h & 0x0000FF00) >> 8;
	data[15] = (_h & 0x000000FF);


	unsigned int scale_factor = 10000;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::DeleteObjectLine(void)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_DEL_OBJECT_LINE;
	int len = 0;
	unsigned char* data = NULL;

	unsigned int scale_factor = 1;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::ClearObjectLine(void)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_CLR_OBJECT_LINE;
	int len = 0;
	unsigned char* data = NULL;

	unsigned int scale_factor = 1;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::SetObjectTwoLine(const int id, float x1, float y1, float w1, float h1, float x2, float y2, float w2, float h2)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_SET_OBJECT_TWO_LINE;

	int len = 4 + 4 + 16*2;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 10000;

	int _x1 = x1 * scale_factor;
	int _y1 = y1 * scale_factor;
	int _w1 = w1 * scale_factor;
	int _h1 = h1 * scale_factor;

	int _x2 = x2 * scale_factor;
	int _y2 = y2 * scale_factor;
	int _w2 = w2 * scale_factor;
	int _h2 = h2 * scale_factor;

	int index = 0;

	//id
	data[index++] = (id) >> 24;
	data[index++] = (id) >> 16;
	data[index++] = (id) >> 8;
	data[index++] = (id);

	//x1
	data[index++] = (_x1 & 0xFF000000) >> 24;
	data[index++] = (_x1 & 0x00FF0000) >> 16;
	data[index++] = (_x1 & 0x0000FF00) >> 8;
	data[index++] = (_x1 & 0x000000FF);

	//y1
	data[index++] = (_y1 & 0xFF000000) >> 24;
	data[index++] = (_y1 & 0x00FF0000) >> 16;
	data[index++] = (_y1 & 0x0000FF00) >> 8;
	data[index++] = (_y1 & 0x000000FF);

	//r1
	data[index++] = (_w1 & 0xFF000000) >> 24;
	data[index++] = (_w1 & 0x00FF0000) >> 16;
	data[index++] = (_w1 & 0x0000FF00) >> 8;
	data[index++] = (_w1 & 0x000000FF);

	//r2
	data[index++] = (_h1 & 0xFF000000) >> 24;
	data[index++] = (_h1 & 0x00FF0000) >> 16;
	data[index++] = (_h1 & 0x0000FF00) >> 8;
	data[index++] = (_h1 & 0x000000FF);

	//x2
	data[index++] = (_x2 & 0xFF000000) >> 24;
	data[index++] = (_x2 & 0x00FF0000) >> 16;
	data[index++] = (_x2 & 0x0000FF00) >> 8;
	data[index++] = (_x2 & 0x000000FF);

	//y2
	data[index++] = (_y2 & 0xFF000000) >> 24;
	data[index++] = (_y2 & 0x00FF0000) >> 16;
	data[index++] = (_y2 & 0x0000FF00) >> 8;
	data[index++] = (_y2 & 0x000000FF);

	//w2
	data[index++] = (_w2 & 0xFF000000) >> 24;
	data[index++] = (_w2 & 0x00FF0000) >> 16;
	data[index++] = (_w2 & 0x0000FF00) >> 8;
	data[index++] = (_w2 & 0x000000FF);

	//h2
	data[index++] = (_h2 & 0xFF000000) >> 24;
	data[index++] = (_h2 & 0x00FF0000) >> 16;
	data[index++] = (_h2 & 0x0000FF00) >> 8;
	data[index++] = (_h2 & 0x000000FF);


	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete[] data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (data != NULL)
	{
		delete[] data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::SetObjectTwoLineBaseDistance(const int id, const int dist)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_SET_OBJECT_TWO_LINE_BASE_DISTANCE;

	int len = 4 + 4;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 1;

	int index = 0;

	//id
	data[index++] = (id & 0xFF000000) >> 24;
	data[index++] = (id & 0x00FF0000) >> 16;
	data[index++] = (id & 0x0000FF00) >> 8;
	data[index++] = (id & 0x000000FF);

	//dist
	data[index++] = (dist & 0xFF000000) >> 24;
	data[index++] = (dist & 0x00FF0000) >> 16;
	data[index++] = (dist & 0x0000FF00) >> 8;
	data[index++] = (dist & 0x000000FF);

	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete[] data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (data != NULL)
	{
		delete[] data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::GetObjectTwoLineBaseDistance(const int id)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GET_OBJECT_TWO_LINE_BASE_DISTANCE;

	int len = 4 ;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 1;

	int index = 0;

	//id
	data[index++] = (id & 0xFF000000) >> 24;
	data[index++] = (id & 0x00FF0000) >> 16;
	data[index++] = (id & 0x0000FF00) >> 8;
	data[index++] = (id & 0x000000FF);

	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete[] data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}

	if (len >= 4)
	{
		//dist
		ret = ((int)data[0] << 24) & 0xFF000000;
		ret |= ((int)data[1] << 16) & 0x00FF0000;
		ret |= ((int)data[2] << 8) & 0x0000FF00;
		ret |= ((int)data[3]) & 0x000000FF;
	}

	if (data != NULL)
	{
		delete[] data;
		data = NULL;
	}

	return ret;
}

float CEyedeaInterface::GetObjectTwoLineCalcDistance(const int id)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GET_OBJECT_TWO_LINE_CALC_DISTANCE;

	int len = 4;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 1;

	int index = 0;

	//id
	data[index++] = (id & 0xFF000000) >> 24;
	data[index++] = (id & 0x00FF0000) >> 16;
	data[index++] = (id & 0x0000FF00) >> 8;
	data[index++] = (id & 0x000000FF);

	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete[] data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}

	float dist = 0;;

	if (len >= 4)
	{
		//dist
		int idist = 0;
		idist = ((int)data[0] << 24) & 0xFF000000;
		idist |= ((int)data[1] << 16) & 0x00FF0000;
		idist |= ((int)data[2] << 8) & 0x0000FF00;
		idist |= ((int)data[3]) & 0x000000FF;

		dist = (float)idist / (float)scale_factor;
	}

	if (data != NULL)
	{
		delete[] data;
		data = NULL;
	}

	return dist;
}

int CEyedeaInterface::SetObjectTwoLineDistanceTolerance(const int id, const int min_value, const int max_value)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_SET_OBJECT_TWO_LINE_DISTANCE_TOL;

	int len = 4 + 4 + 4;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 1;

	int index = 0;

	//id
	data[index++] = (id & 0xFF000000) >> 24;
	data[index++] = (id & 0x00FF0000) >> 16;
	data[index++] = (id & 0x0000FF00) >> 8;
	data[index++] = (id & 0x000000FF);

	//min
	data[index++] = (min_value & 0xFF000000) >> 24;
	data[index++] = (min_value & 0x00FF0000) >> 16;
	data[index++] = (min_value & 0x0000FF00) >> 8;
	data[index++] = (min_value & 0x000000FF);

	//max_valuie
	data[index++] = (max_value & 0xFF000000) >> 24;
	data[index++] = (max_value & 0x00FF0000) >> 16;
	data[index++] = (max_value & 0x0000FF00) >> 8;
	data[index++] = (max_value & 0x000000FF);

	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete[] data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (data != NULL)
	{
		delete[] data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::GetObjectTwoLineDistanceTolerance(const int id, int *out_min_value, int *out_max_value)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GET_OBJECT_TWO_LINE_DISTANCE_TOL;

	int len = 4;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 1;

	int index = 0;

	//id
	data[index++] = (id & 0xFF000000) >> 24;
	data[index++] = (id & 0x00FF0000) >> 16;
	data[index++] = (id & 0x0000FF00) >> 8;
	data[index++] = (id & 0x000000FF);

	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete[] data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}

	int min_value = 0;
	int max_value = 0;

	if (len >= 8)
	{
		int index = 0;

		min_value = ((int)data[index++] << 24) & 0xFF000000;
		min_value |= ((int)data[index++] << 16) & 0x00FF0000;
		min_value |= ((int)data[index++] << 8) & 0x0000FF00;
		min_value |= ((int)data[index++]) & 0x000000FF;

		max_value = ((int)data[index++] << 24) & 0xFF000000;
		max_value |= ((int)data[index++] << 16) & 0x00FF0000;
		max_value |= ((int)data[index++] << 8) & 0x0000FF00;
		max_value |= ((int)data[index++]) & 0x000000FF;
	}

	if (data != NULL)
	{
		delete[] data;
		data = NULL;
	}

	if (out_min_value != NULL) (*out_min_value) = min_value;
	if (out_max_value != NULL) (*out_max_value) = max_value;

	return ret;
}

int CEyedeaInterface::SetObjectTwoLineDistanceType(const int id, const int type)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_SET_OBJECT_TWO_LINE_DISTANCE_TYPE;

	int len = 4 + 4;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 1;

	int index = 0;

	//id
	data[index++] = (id & 0xFF000000) >> 24;
	data[index++] = (id & 0x00FF0000) >> 16;
	data[index++] = (id & 0x0000FF00) >> 8;
	data[index++] = (id & 0x000000FF);

	//min
	data[index++] = (type & 0xFF000000) >> 24;
	data[index++] = (type & 0x00FF0000) >> 16;
	data[index++] = (type & 0x0000FF00) >> 8;
	data[index++] = (type & 0x000000FF);

	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete[] data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (data != NULL)
	{
		delete[] data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::GetObjectTwoLineDistanceType(const int id)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GET_OBJECT_TWO_LINE_DISTANCE_TYPE;

	int len = 4;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 1;

	int index = 0;

	//id
	data[index++] = (id & 0xFF000000) >> 24;
	data[index++] = (id & 0x00FF0000) >> 16;
	data[index++] = (id & 0x0000FF00) >> 8;
	data[index++] = (id & 0x000000FF);

	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete[] data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}

	int type = 0;

	if (len >= 4)
	{
		int index = 0;

		type = ((int)data[index++] << 24) & 0xFF000000;
		type |= ((int)data[index++] << 16) & 0x00FF0000;
		type |= ((int)data[index++] << 8) & 0x0000FF00;
		type |= ((int)data[index++]) & 0x000000FF;

	}

	if (data != NULL)
	{
		delete[] data;
		data = NULL;
	}

	return type;
}

int CEyedeaInterface::SetObjectTwoLineDistanceInspection(const int id, const bool use)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_SET_OBJECT_TWO_LINE_DISTANCE_INSPECTION_USE;

	int len = 4 + 4;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 1;

	int index = 0;

	//id
	data[index++] = (id & 0xFF000000) >> 24;
	data[index++] = (id & 0x00FF0000) >> 16;
	data[index++] = (id & 0x0000FF00) >> 8;
	data[index++] = (id & 0x000000FF);

	//use
	data[index++] = (use & 0xFF000000) >> 24;
	data[index++] = (use & 0x00FF0000) >> 16;
	data[index++] = (use & 0x0000FF00) >> 8;
	data[index++] = (use & 0x000000FF);

	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete[] data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (data != NULL)
	{
		delete[] data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::GetObjectTwoLineDistanceInspection(const int id)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GET_OBJECT_TWO_LINE_DISTANCE_INSPECTION_USE;

	int len = 4;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 1;

	int index = 0;

	//id
	data[index++] = (id & 0xFF000000) >> 24;
	data[index++] = (id & 0x00FF0000) >> 16;
	data[index++] = (id & 0x0000FF00) >> 8;
	data[index++] = (id & 0x000000FF);

	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete[] data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}

	int use = 0;

	if (len >= 4)
	{
		int index = 0;

		use = ((int)data[index++] << 24) & 0xFF000000;
		use |= ((int)data[index++] << 16) & 0x00FF0000;
		use |= ((int)data[index++] << 8) & 0x0000FF00;
		use |= ((int)data[index++]) & 0x000000FF;

	}

	if (data != NULL)
	{
		delete[] data;
		data = NULL;
	}

	return use;
}

int CEyedeaInterface::SetNextImage(void)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_SET_NEXT_IMAGE;
	unsigned char* data = new unsigned char[1];

	data[0] = 0x01;		//set option - pca on

	int len = 1;

	unsigned int scale_factor = 1;
	int ret = 0;
    ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if(ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if(ret == 0){
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

    return ret;
}

int CEyedeaInterface::OptionFixSearchAreaOn(void)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_SET_OPTION;
	unsigned char* data = new unsigned char[1];

	data[0] = 0x03;		//set option - OptionFixSearchAreaOn

	int len = 1;

	unsigned int scale_factor = 1;
	int ret = 0;
    ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if(ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if(ret == 0){
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}if (data != NULL)
				{
					delete data;
					data = NULL;
				}
				continue;
			}
		}
	}
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

    return ret;
}

int CEyedeaInterface::OptionFixSearchAreaOff(void)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_SET_OPTION;
	unsigned char* data = new unsigned char[1];

	data[0] = 0x04;		//set option - OptionFixSearchAreaOn

	int len = 1;

	unsigned int scale_factor = 1;
	int ret = 0;
    ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if(ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if(ret == 0){
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
				
					return ret;
				}
				continue;
			}
		}
	}
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

    return ret;
}

int CEyedeaInterface::GetOptionFixSearchArea(void)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GET_OPTION;
	unsigned char* data = new unsigned char[1];

	data[0] = 0x01;		//get status of fix search area option 

	int len = 1;

	unsigned int scale_factor = 1;
	int ret = 0;
    ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if(ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if(ret == 0){
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}

					return ret;
				}
				continue;
			}
		}
	}
	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}
		return ret;
	}

	ret = 0;
	
	if (len >= 1)
	{
		ret = (int)data[0];
	}

	//printf("int ret = 0;	//printf("len = %d\n", len);

	delete data;
	data = NULL;

	return ret ;

}

int CEyedeaInterface::FileLoadObjectListInfo(std::string path)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_OBJECT_LIST_FILELOAD;

	int len = path.length() + 1;
	unsigned char* data = new unsigned char[len];
	memset(data, 0, len);

	std::copy(path.begin(), path.end(), data);

	//id
#if 0
	data[0] = (id & 0xFF000000) >> 24;
	data[1] = (id & 0x00FF0000) >> 16;
	data[2] = (id & 0x0000FF00) >> 8;
	data[3] = (id & 0x000000FF);
#endif

	unsigned int scale_factor = 1;
	int ret = 0;
    ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if(ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if(ret == 0){
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}

					return ret;
				}
				continue;
			}
		}
	}
	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}
		return ret;
	}

	ret = 0;

	if (len >= 4)
	{
		//w
		ret = ((int)data[0] << 24) & 0xFF000000;
		ret |= ((int)data[1] << 16) & 0x00FF0000;
		ret |= ((int)data[2] << 8) & 0x0000FF00;
		ret |= ((int)data[3]) & 0x000000FF;
	}

	delete data;
	data = NULL;

	return ret;
}

int CEyedeaInterface::FileSaveObjectListInfo(std::string path)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_OBJECT_LIST_FILESAVE;

	int len = path.length()+1;
	unsigned char* data = new unsigned char[len];
	memset(data, 0, len);

	std::copy(path.begin(), path.end(), data);

	//id
#if 0
	data[0] = (id & 0xFF000000) >> 24;
	data[1] = (id & 0x00FF0000) >> 16;
	data[2] = (id & 0x0000FF00) >> 8;
	data[3] = (id & 0x000000FF);
#endif

	unsigned int scale_factor = 1;
	int ret = 0;
    ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if(ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if(ret == 0){
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}
		return ret;
	}

    ret = 0;

	if (len >= 1)
	{
		ret = (int)data[0];
	}

	delete data;
	data = NULL;

	return ret;
}

int CEyedeaInterface::SetBase(int dep_id)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	//COMAMND_RUN_FIND_OBJECT_WITHOUT_PCA
	char command = COMMAND_SET_BASE;
	int len = 4;
	unsigned char* data = new unsigned char[len];

	//id
	data[0] = (dep_id & 0xFF000000) >> 24;
	data[1] = (dep_id & 0x00FF0000) >> 16;
	data[2] = (dep_id & 0x0000FF00) >> 8;
	data[3] = (dep_id & 0x000000FF);


	unsigned int scale_factor = 1;
	int ret = 0;
    ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if(ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if(ret == 0){
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

    return ret;
}

int CEyedeaInterface::CreateSocket(char* ip, int port)
{
	m_ip = new char[strlen(ip)];
	strcpy(m_ip, ip);
	m_port = port;

	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client)
	{
		//m_cls_eth_client->Close();

		//delete m_cls_eth_client;
		//m_cls_eth_client = NULL;
        return EYEDEA_ERROR_ALREADY_CONNECT;
	}

	m_cls_eth_client = new CEyedeaEthernetClient();

	int ret;

	//int sec = 0;

	/*
	while(1){
		ret = m_cls_eth_client->Open(ip, port);
		if(ret == 0)
			break;
		else
		{
			boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
			sec++;
			if(sec >= 60)
				break;
			continue;
		}
	}
	//return m_cls_eth_client->Open(ip, port);
	*/
	ret = m_cls_eth_client->Open(ip, port);

	if (ret != 0)
	{
		delete m_cls_eth_client;
		m_cls_eth_client = NULL;
	}

	return ret;
}

void CEyedeaInterface::DeleteSocket()
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return;
	}

	m_cls_eth_client->Close();
    delete(m_cls_eth_client);
    m_cls_eth_client = NULL;
}

int CEyedeaInterface::Calibration_GetID(void)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	//printf("SetSearchArea - %d %d %d %d\n", x, y, w, h);

	char command = COMMAND_CALIBRATION_GET_ID;
	int len = 0;
	unsigned char* data = NULL;

	unsigned int scale_factor = 1;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}

		return ret;
	}

	ret = 0;

	if (len >= 4)
	{
		//w
		ret = ((int)data[0] << 24) & 0xFF000000;
		ret |= ((int)data[1] << 16) & 0x00FF0000;
		ret |= ((int)data[2] << 8) & 0x0000FF00;
		ret |= ((int)data[3]) & 0x000000FF;

	}

	delete (data);
	data = NULL;

	return ret;
}

int CEyedeaInterface::Calibration_isOK(void)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	//printf("SetSearchArea - %d %d %d %d\n", x, y, w, h);

	char command = COMMAND_CALIBRATION_IS_OK;
	int len = 0;
	unsigned char* data = NULL;

	unsigned int scale_factor = 1;
	int ret = 0;
    ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if(ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if(ret == 0){
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}

					return ret;
				}
				continue;
			}
		}
	}
	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}

		return ret;
	}

	ret =0;

	if (len >= 4)
	{
		//w
		ret = ((int)data[0] << 24) & 0xFF000000;
		ret |= ((int)data[1] << 16) & 0x00FF0000;
		ret |= ((int)data[2] << 8) & 0x0000FF00;
		ret |= ((int)data[3]) & 0x000000FF;

	}

	delete (data);
	data = NULL;

	return ret;
}

int CEyedeaInterface::Calibration_Add(float robot_x, float robot_y)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	//printf("SetSearchArea - %d %d %d %d\n", x, y, w, h);

	char command = COMMAND_CALIBRATION_ADD;
	int len = 8;
	unsigned char* data = new unsigned char[len];

	int i_robot_x = robot_x * 10000.0 ;
	int i_robot_y = robot_y * 10000.0 ;

	//robot_x
	data[0] = (i_robot_x & 0xFF000000) >> 24;
	data[1] = (i_robot_x & 0x00FF0000) >> 16;
	data[2] = (i_robot_x & 0x0000FF00) >> 8;
	data[3] = (i_robot_x & 0x000000FF);

	//robot_y
	data[4] = (i_robot_y & 0xFF000000) >> 24;
	data[5] = (i_robot_y & 0x00FF0000) >> 16;
	data[6] = (i_robot_y & 0x0000FF00) >> 8;
	data[7] = (i_robot_y & 0x000000FF);

	unsigned int scale_factor = 10000;
	int ret = 0;
    ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if(ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if(ret == 0){
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}

	if (data != NULL)
	{
		delete (data);
		data = NULL;
	}

    return ret;
}

int CEyedeaInterface::Calibration_GetCount(void)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	//printf("SetSearchArea - %d %d %d %d\n", x, y, w, h);

	char command = COMMAND_CALIBRATION_GET_COUNT;
	int len = 0;
	unsigned char* data = NULL;

	unsigned int scale_factor = 1;
	int size = 0;
    size = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if(size == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			size = m_cls_eth_client->Open(m_ip, m_port);
			if(size == 0){
				size = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}

					return size;
				}
				continue;
			}
		}
	}
	if (size != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}
		return size;
	}

	size = 0;

	if (len >= 4)
	{
		//w
		size = ((int)data[0] << 24) & 0xFF000000;
		size |= ((int)data[1] << 16) & 0x00FF0000;
		size |= ((int)data[2] << 8) & 0x0000FF00;
		size |= ((int)data[3]) & 0x000000FF;

	}

	delete (data);
	data = NULL;

	return size;
}

int CEyedeaInterface::Calibration_GetImage(int index, char** out_data, int* len)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_CALIBRATION_GET_IMAGE;

	//option
	(*out_data)[0] = (index & 0xFF000000) >> 24;
	(*out_data)[1] = (index & 0x00FF0000) >> 16;
	(*out_data)[2] = (index & 0x0000FF00) >> 8;
	(*out_data)[3] = (index & 0x000000FF);

	unsigned int scale_factor = 1;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, (unsigned char **)out_data, len);
	if(ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if(ret == 0){
				ret = m_cls_eth_client->Send(command, &scale_factor, (unsigned char **)out_data, len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if(sec >= 60)
					return ret;
				continue;
			}
		}
	}
    return ret;
}

int CEyedeaInterface::Calibration_GetRobotInfo(int index, float *out_robot_x, float *out_robot_y)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_CALIBRATION_GET_INFO;

	int len = 4;
	unsigned char* data = new unsigned char[len];

	//index
	data[0] = (index & 0xFF000000) >> 24;
	data[1] = (index & 0x00FF0000) >> 16;
	data[2] = (index & 0x0000FF00) >> 8;
	data[3] = (index & 0x000000FF);

	unsigned int scale_factor = 1;
	int ret = 0;
    ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if(ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if(ret == 0){
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}
		return ret;
	}

	int i_robot_x = 0;
	int i_robot_y = 0;

	if (len >= 8)
	{
		//i_robot_x
		i_robot_x = ((int)data[0] << 24) & 0xFF000000;
		i_robot_x |= ((int)data[1] << 16) & 0x00FF0000;
		i_robot_x |= ((int)data[2] << 8) & 0x0000FF00;
		i_robot_x |= ((int)data[3]) & 0x000000FF;

		//i_robot_y
		i_robot_y = ((int)data[4] << 24) & 0xFF000000;
		i_robot_y |= ((int)data[5] << 16) & 0x00FF0000;
		i_robot_y |= ((int)data[6] << 8) & 0x0000FF00;
		i_robot_y |= ((int)data[7]) & 0x000000FF;
	}

	(*out_robot_x) = (float)i_robot_x / (float)scale_factor;
	(*out_robot_y) = (float)i_robot_y / (float)scale_factor;

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

    return ret;
}

int CEyedeaInterface::Calibration_Del(int index)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_CALIBRATION_DEL;

	int len = 4;
	unsigned char* data = new unsigned char[len];

	//index
	data[0] = (index & 0xFF000000) >> 24;
	data[1] = (index & 0x00FF0000) >> 16;
	data[2] = (index & 0x0000FF00) >> 8;
	data[3] = (index & 0x000000FF);

	unsigned int scale_factor = 1;
	int ret = 0;
    ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if(ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if(ret == 0){
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

    return ret;
}

int CEyedeaInterface::Calibration_Clear(void)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_CALIBRATION_CLEAR;

	int len = 0;
	unsigned char* data = NULL;

	unsigned int scale_factor = 1;
	int ret = 0;
    ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if(ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if(ret == 0){
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

    return ret;
}

int CEyedeaInterface::Calibration_Run(void)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_CALIBRATION_RUN;

	int len = 0;
	unsigned char* data = NULL;

	unsigned int scale_factor = 1;
	int ret = 0;
    ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if(ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if(ret == 0){
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}
		return ret;
	}

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

    return ret;
}

int CEyedeaInterface::Calibration_GetPoint(const float in_px, const float in_py, float* out_rx, float* out_ry)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	//printf("SetSearchArea - %d %d %d %d\n", x, y, w, h);

	char command = COMMAND_CALIBRATION_GEPOINT;

	int len = 8;
	unsigned char* data = new unsigned char[8];

	int _x = in_px * 1000;
	int _y = in_py * 1000;

	//x
	data[0] = (_x & 0xFF000000) >> 24;
	data[1] = (_x & 0x00FF0000) >> 16;
	data[2] = (_x & 0x0000FF00) >> 8;
	data[3] = (_x & 0x000000FF);

	//y
	data[4] = (_y & 0xFF000000) >> 24;
	data[5] = (_y & 0x00FF0000) >> 16;
	data[6] = (_y & 0x0000FF00) >> 8;
	data[7] = (_y & 0x000000FF);

	unsigned int scale_factor = 1000;
	int ret = 0;
    ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if(ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if(ret == 0){
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}
		return ret;
	}

	int i_robot_x = 0;
	int i_robot_y = 0;

	if (len >= 8)
	{
		//i_robot_x
		i_robot_x = ((int)data[0] << 24) & 0xFF000000;
		i_robot_x |= ((int)data[1] << 16) & 0x00FF0000;
		i_robot_x |= ((int)data[2] << 8) & 0x0000FF00;
		i_robot_x |= ((int)data[3]) & 0x000000FF;

		//i_robot_y
		i_robot_y = ((int)data[4] << 24) & 0xFF000000;
		i_robot_y |= ((int)data[5] << 16) & 0x00FF0000;
		i_robot_y |= ((int)data[6] << 8) & 0x0000FF00;
		i_robot_y |= ((int)data[7]) & 0x000000FF;
	}

	(*out_rx) = (float)i_robot_x / (float)scale_factor;
	(*out_ry) = (float)i_robot_y / (float)scale_factor;

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

    return ret;
}

int CEyedeaInterface::Calibration_GetChessPoint(const int index, float* out_rx, float* out_ry)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	//printf("SetSearchArea - %d %d %d %d\n", x, y, w, h);

	char command = COMMAND_CALIBRATION_GETCHESSPOINT;

	int len = 4;
	unsigned char* data = new unsigned char[len];

	//index
	data[0] = (index & 0xFF000000) >> 24;
	data[1] = (index & 0x00FF0000) >> 16;
	data[2] = (index & 0x0000FF00) >> 8;
	data[3] = (index & 0x000000FF);

	unsigned int scale_factor = 1000;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}
		return ret;
	}

	int i_robot_x = 0;
	int i_robot_y = 0;

	if (len >= 8)
	{
		//i_robot_x
		i_robot_x = ((int)data[0] << 24) & 0xFF000000;
		i_robot_x |= ((int)data[1] << 16) & 0x00FF0000;
		i_robot_x |= ((int)data[2] << 8) & 0x0000FF00;
		i_robot_x |= ((int)data[3]) & 0x000000FF;

		//i_robot_y
		i_robot_y = ((int)data[4] << 24) & 0xFF000000;
		i_robot_y |= ((int)data[5] << 16) & 0x00FF0000;
		i_robot_y |= ((int)data[6] << 8) & 0x0000FF00;
		i_robot_y |= ((int)data[7]) & 0x000000FF;
	}

	(*out_rx) = (float)i_robot_x / (float)scale_factor;
	(*out_ry) = (float)i_robot_y / (float)scale_factor;

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::GetFindObjectResultInfo(int base_index, int sub_index, float* out_id, float* out_cx, float* out_cy, float* out_rx, float* out_ry, float* out_angle, float* out_type, float* out_score, float** out_histogram, float** out_histogram_b, float** out_histogram_g, float** out_histogram_r, float* out_histogram_size)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GET_FIND_OBJECT_INFO_RESULT;

	int len = 8;
	unsigned char* data = new unsigned char[len];

	//index
	int data_index = 0;
	data[data_index++] = (base_index & 0xFF000000) >> 24;
	data[data_index++] = (base_index & 0x00FF0000) >> 16;
	data[data_index++] = (base_index & 0x0000FF00) >> 8;
	data[data_index++] = (base_index & 0x000000FF);

	//size
	data[data_index++] = (sub_index & 0xFF000000) >> 24;
	data[data_index++] = (sub_index & 0x00FF0000) >> 16;
	data[data_index++] = (sub_index & 0x0000FF00) >> 8;
	data[data_index++] = (sub_index & 0x000000FF);

	unsigned int scale_factor = 1;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (ret != 0)
	{

		if (data != NULL)
		{
			delete data;
			data = NULL;
		}
		return ret;
	}

	int i_id = 0;
	int i_camera_x = 0;
	int i_camera_y = 0;
	int i_robot_x = 0;
	int i_robot_y = 0;
	int i_angle = 0;
	int i_type = 0;
	int i_score = 0;
	int i_histogram_size = 0;
	int i_histogram_data = 0;

	int index = 0;

	if (len >= 4)
	{
		//i_id
		i_id = ((int)data[index++] << 24) & 0xFF000000;
		i_id |= ((int)data[index++] << 16) & 0x00FF0000;
		i_id |= ((int)data[index++] << 8) & 0x0000FF00;
		i_id |= ((int)data[index++]) & 0x000000FF;

		//i_camera_x
		i_camera_x = ((int)data[index++] << 24) & 0xFF000000;
		i_camera_x |= ((int)data[index++] << 16) & 0x00FF0000;
		i_camera_x |= ((int)data[index++] << 8) & 0x0000FF00;
		i_camera_x |= ((int)data[index++]) & 0x000000FF;

		//i_camera_y
		i_camera_y = ((int)data[index++] << 24) & 0xFF000000;
		i_camera_y |= ((int)data[index++] << 16) & 0x00FF0000;
		i_camera_y |= ((int)data[index++] << 8) & 0x0000FF00;
		i_camera_y |= ((int)data[index++]) & 0x000000FF;

		//i_robot_x
		i_robot_x = ((int)data[index++] << 24) & 0xFF000000;
		i_robot_x |= ((int)data[index++] << 16) & 0x00FF0000;
		i_robot_x |= ((int)data[index++] << 8) & 0x0000FF00;
		i_robot_x |= ((int)data[index++]) & 0x000000FF;

		//i_robot_y
		i_robot_y = ((int)data[index++] << 24) & 0xFF000000;
		i_robot_y |= ((int)data[index++] << 16) & 0x00FF0000;
		i_robot_y |= ((int)data[index++] << 8) & 0x0000FF00;
		i_robot_y |= ((int)data[index++]) & 0x000000FF;

		//i_angle
		i_angle = ((int)data[index++] << 24) & 0xFF000000;
		i_angle |= ((int)data[index++] << 16) & 0x00FF0000;
		i_angle |= ((int)data[index++] << 8) & 0x0000FF00;
		i_angle |= ((int)data[index++]) & 0x000000FF;

		//i_type
		i_type = ((int)data[index++] << 24) & 0xFF000000;
		i_type |= ((int)data[index++] << 16) & 0x00FF0000;
		i_type |= ((int)data[index++] << 8) & 0x0000FF00;
		i_type |= ((int)data[index++]) & 0x000000FF;

		//i_score
		i_score = ((int)data[index++] << 24) & 0xFF000000;
		i_score |= ((int)data[index++] << 16) & 0x00FF0000;
		i_score |= ((int)data[index++] << 8) & 0x0000FF00;
		i_score |= ((int)data[index++]) & 0x000000FF;

		//i_histogram_size
		i_histogram_size = ((int)data[index++] << 24) & 0xFF000000;
		i_histogram_size |= ((int)data[index++] << 16) & 0x00FF0000;
		i_histogram_size |= ((int)data[index++] << 8) & 0x0000FF00;
		i_histogram_size |= ((int)data[index++]) & 0x000000FF;

		(*out_id) = (float)i_id / (float)scale_factor;
		(*out_cx) = (float)i_camera_x / (float)scale_factor;
		(*out_cy) = (float)i_camera_y / (float)scale_factor;
		(*out_rx) = (float)i_robot_x / (float)scale_factor;
		(*out_ry) = (float)i_robot_y / (float)scale_factor;
		(*out_angle) = (float)i_angle / (float)scale_factor;
		(*out_type) = (float)i_type / (float)scale_factor;
		(*out_score) = (float)i_score / (float)scale_factor;
		(*out_histogram_size) = (float)i_histogram_size / (float)scale_factor;

		int histogram_size = (int)(*out_histogram_size);
		if (histogram_size > 0)
		{
			//gray
			if ((*out_histogram) != NULL)	free((*out_histogram));
			(*out_histogram) = (float *)malloc(sizeof(float)*histogram_size);

			for (int i = 0; i < histogram_size; i++)
			{
				//i_score
				i_histogram_data = ((int)data[index++] << 24) & 0xFF000000;
				i_histogram_data |= ((int)data[index++] << 16) & 0x00FF0000;
				i_histogram_data |= ((int)data[index++] << 8) & 0x0000FF00;
				i_histogram_data |= ((int)data[index++]) & 0x000000FF;

				(*out_histogram)[i] = (float)i_histogram_data / (float)scale_factor;
			}

			//b
			if ((*out_histogram_b) != NULL)	free((*out_histogram_b));
			(*out_histogram_b) = (float *)malloc(sizeof(float)*histogram_size);

			for (int i = 0; i < histogram_size; i++)
			{
				//i_score
				i_histogram_data = ((int)data[index++] << 24) & 0xFF000000;
				i_histogram_data |= ((int)data[index++] << 16) & 0x00FF0000;
				i_histogram_data |= ((int)data[index++] << 8) & 0x0000FF00;
				i_histogram_data |= ((int)data[index++]) & 0x000000FF;

				(*out_histogram_b)[i] = (float)i_histogram_data / (float)scale_factor;
			}

			//g
			if ((*out_histogram_g) != NULL)	free((*out_histogram_g));
			(*out_histogram_g) = (float *)malloc(sizeof(float)*histogram_size);

			for (int i = 0; i < histogram_size; i++)
			{
				//i_score
				i_histogram_data = ((int)data[index++] << 24) & 0xFF000000;
				i_histogram_data |= ((int)data[index++] << 16) & 0x00FF0000;
				i_histogram_data |= ((int)data[index++] << 8) & 0x0000FF00;
				i_histogram_data |= ((int)data[index++]) & 0x000000FF;

				(*out_histogram_g)[i] = (float)i_histogram_data / (float)scale_factor;
			}

			//r
			if ((*out_histogram_r) != NULL)	free((*out_histogram_r));
			(*out_histogram_r) = (float *)malloc(sizeof(float)*histogram_size);

			for (int i = 0; i < histogram_size; i++)
			{
				//i_score
				i_histogram_data = ((int)data[index++] << 24) & 0xFF000000;
				i_histogram_data |= ((int)data[index++] << 16) & 0x00FF0000;
				i_histogram_data |= ((int)data[index++] << 8) & 0x0000FF00;
				i_histogram_data |= ((int)data[index++]) & 0x000000FF;

				(*out_histogram_r)[i] = (float)i_histogram_data / (float)scale_factor;
			}
		}
	}


	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return i_histogram_size;
}

int CEyedeaInterface::GetFindObjectInfo(int index, int max_objects_count, int option, float** out_id, float** out_cx, float** out_cy, float** out_rx, float** out_ry, float** out_bound_cx, float** out_bound_cy, float** out_bound_rx, float** out_bound_ry, float** out_mass_cx, float** out_mass_cy, float** out_mass_rx, float** out_mass_ry, float** out_circle_rx, float** out_circle_ry, float** out_line1_x, float** out_line1_y, float** out_line2_x, float** out_line2_y, float** out_angle, float** out_type, float** out_score, float** out_pass)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GET_FIND_OBJECT_INFO;

	int len = 4 + 4 + 4;
	unsigned char* data = new unsigned char[len];

	//index
	int data_index = 0;
	data[data_index++] = (index & 0xFF000000) >> 24;
	data[data_index++] = (index & 0x00FF0000) >> 16;
	data[data_index++] = (index & 0x0000FF00) >> 8;
	data[data_index++] = (index & 0x000000FF);

	//max_objects_count
	data[data_index++] = (max_objects_count & 0xFF000000) >> 24;
	data[data_index++] = (max_objects_count & 0x00FF0000) >> 16;
	data[data_index++] = (max_objects_count & 0x0000FF00) >> 8;
	data[data_index++] = (max_objects_count & 0x000000FF);

	//option
	data[data_index++] = (option & 0xFF000000) >> 24;
	data[data_index++] = (option & 0x00FF0000) >> 16;
	data[data_index++] = (option & 0x0000FF00) >> 8;
	data[data_index++] = (option & 0x000000FF);

	unsigned int scale_factor = 1;
	int ret = 0;
    ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if(ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if(ret == 0){
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}
		return ret;
	}

	int i_id = 0;
	int i_camera_x = 0;
	int i_camera_y = 0;
	int i_robot_x = 0;
	int i_robot_y = 0;
	int i_camera_bound_x = 0;
	int i_camera_bound_y = 0;
	int i_robot_bound_x = 0;
	int i_robot_bound_y = 0;
	int i_camera_mass_x = 0;
	int i_camera_mass_y = 0;
	int i_robot_mass_x = 0;
	int i_robot_mass_y = 0;
	int i_circle_rx = 0;
	int i_circle_ry = 0;
	int i_line1_x = 0;
	int i_line1_y = 0;
	int i_line2_x = 0;
	int i_line2_y = 0;
	int i_angle = 0;
	int i_type = 0;
	int i_score = 0;
	int i_pass = 0;

	int nObject = 0;
	//int index = 0;
	index = 0;

	if (len >= 4)
	{
		//nObject
		nObject = ((int)data[index++] << 24) & 0xFF000000;
		nObject |= ((int)data[index++] << 16) & 0x00FF0000;
		nObject |= ((int)data[index++] << 8) & 0x0000FF00;
		nObject |= ((int)data[index++]) & 0x000000FF;

		if (nObject > 0 && len >= 4 + ((23*4)* nObject))
		{
			if ((*out_id) != NULL)	free((*out_id));
			if ((*out_cx) != NULL)	free((*out_cx));
			if ((*out_cy) != NULL)	free((*out_cy));
			if ((*out_rx) != NULL)	free((*out_rx));
			if ((*out_ry) != NULL)	free((*out_ry));
			if ((*out_bound_cx) != NULL)	free((*out_bound_cx));
			if ((*out_bound_cy) != NULL)	free((*out_bound_cy));
			if ((*out_bound_rx) != NULL)	free((*out_bound_rx));
			if ((*out_bound_ry) != NULL)	free((*out_bound_ry));
			if ((*out_mass_cx) != NULL)	free((*out_mass_cx));
			if ((*out_mass_cy) != NULL)	free((*out_mass_cy));
			if ((*out_mass_rx) != NULL)	free((*out_mass_rx));
			if ((*out_mass_ry) != NULL)	free((*out_mass_ry));
			if ((*out_circle_rx) != NULL)	free((*out_circle_rx));
			if ((*out_circle_ry) != NULL)	free((*out_circle_ry));
			if ((*out_line1_x) != NULL)	free((*out_line1_x));
			if ((*out_line1_y) != NULL)	free((*out_line1_y));
			if ((*out_line2_x) != NULL)	free((*out_line2_x));
			if ((*out_line2_y) != NULL)	free((*out_line2_y));
			if ((*out_angle) != NULL)	free((*out_angle));
			if ((*out_type) != NULL)	free((*out_type));
			if ((*out_score) != NULL)	free((*out_score));
			if ((*out_pass) != NULL)	free((*out_pass));
			//if ((*out_index) != NULL)	free((*out_index));

			(*out_id) = (float *)malloc(sizeof(float)*nObject);
			(*out_cx) = (float *)malloc(sizeof(float)*nObject);
			(*out_cy) = (float *)malloc(sizeof(float)*nObject);
			(*out_rx) = (float *)malloc(sizeof(float)*nObject);
			(*out_ry) = (float *)malloc(sizeof(float)*nObject);
			(*out_bound_cx) = (float *)malloc(sizeof(float)*nObject);
			(*out_bound_cy) = (float *)malloc(sizeof(float)*nObject);
			(*out_bound_rx) = (float *)malloc(sizeof(float)*nObject);
			(*out_bound_ry) = (float *)malloc(sizeof(float)*nObject);
			(*out_mass_cx) = (float *)malloc(sizeof(float)*nObject);
			(*out_mass_cy) = (float *)malloc(sizeof(float)*nObject);
			(*out_mass_rx) = (float *)malloc(sizeof(float)*nObject);
			(*out_mass_ry) = (float *)malloc(sizeof(float)*nObject);
			(*out_circle_rx) = (float *)malloc(sizeof(float)*nObject);
			(*out_circle_ry) = (float *)malloc(sizeof(float)*nObject);
			(*out_line1_x) = (float *)malloc(sizeof(float)*nObject);
			(*out_line1_y) = (float *)malloc(sizeof(float)*nObject);
			(*out_line2_x) = (float *)malloc(sizeof(float)*nObject);
			(*out_line2_y) = (float *)malloc(sizeof(float)*nObject);
			(*out_angle) = (float *)malloc(sizeof(float)*nObject);
			(*out_type) = (float *)malloc(sizeof(float)*nObject);
			(*out_score) = (float *)malloc(sizeof(float)*nObject);
			(*out_pass) = (float *)malloc(sizeof(float)*nObject);
			//(*out_index) = (float *)malloc(sizeof(float)*nObject);

			for (int i = 0; i < nObject; i++)
			{
				//i_id
				i_id = ((int)data[index++] << 24) & 0xFF000000;
				i_id |= ((int)data[index++] << 16) & 0x00FF0000;
				i_id |= ((int)data[index++] << 8) & 0x0000FF00;
				i_id |= ((int)data[index++]) & 0x000000FF;

				//i_camera_x
				i_camera_x = ((int)data[index++] << 24) & 0xFF000000;
				i_camera_x |= ((int)data[index++] << 16) & 0x00FF0000;
				i_camera_x |= ((int)data[index++] << 8) & 0x0000FF00;
				i_camera_x |= ((int)data[index++]) & 0x000000FF;

				//i_camera_y
				i_camera_y = ((int)data[index++] << 24) & 0xFF000000;
				i_camera_y |= ((int)data[index++] << 16) & 0x00FF0000;
				i_camera_y |= ((int)data[index++] << 8) & 0x0000FF00;
				i_camera_y |= ((int)data[index++]) & 0x000000FF;

				//i_robot_x
				i_robot_x = ((int)data[index++] << 24) & 0xFF000000;
				i_robot_x |= ((int)data[index++] << 16) & 0x00FF0000;
				i_robot_x |= ((int)data[index++] << 8) & 0x0000FF00;
				i_robot_x |= ((int)data[index++]) & 0x000000FF;

				//i_robot_y
				i_robot_y = ((int)data[index++] << 24) & 0xFF000000;
				i_robot_y |= ((int)data[index++] << 16) & 0x00FF0000;
				i_robot_y |= ((int)data[index++] << 8) & 0x0000FF00;
				i_robot_y |= ((int)data[index++]) & 0x000000FF;

				//bound center
				//i_camera_x
				i_camera_bound_x = ((int)data[index++] << 24) & 0xFF000000;
				i_camera_bound_x |= ((int)data[index++] << 16) & 0x00FF0000;
				i_camera_bound_x |= ((int)data[index++] << 8) & 0x0000FF00;
				i_camera_bound_x |= ((int)data[index++]) & 0x000000FF;

				//i_camera_y
				i_camera_bound_y = ((int)data[index++] << 24) & 0xFF000000;
				i_camera_bound_y |= ((int)data[index++] << 16) & 0x00FF0000;
				i_camera_bound_y |= ((int)data[index++] << 8) & 0x0000FF00;
				i_camera_bound_y |= ((int)data[index++]) & 0x000000FF;

				//i_robot_x
				i_robot_bound_x = ((int)data[index++] << 24) & 0xFF000000;
				i_robot_bound_x |= ((int)data[index++] << 16) & 0x00FF0000;
				i_robot_bound_x |= ((int)data[index++] << 8) & 0x0000FF00;
				i_robot_bound_x |= ((int)data[index++]) & 0x000000FF;

				//i_robot_y
				i_robot_bound_y = ((int)data[index++] << 24) & 0xFF000000;
				i_robot_bound_y |= ((int)data[index++] << 16) & 0x00FF0000;
				i_robot_bound_y |= ((int)data[index++] << 8) & 0x0000FF00;
				i_robot_bound_y |= ((int)data[index++]) & 0x000000FF;

				//center of mass
				//i_camera_x
				i_camera_mass_x = ((int)data[index++] << 24) & 0xFF000000;
				i_camera_mass_x |= ((int)data[index++] << 16) & 0x00FF0000;
				i_camera_mass_x |= ((int)data[index++] << 8) & 0x0000FF00;
				i_camera_mass_x |= ((int)data[index++]) & 0x000000FF;

				//i_camera_y
				i_camera_mass_y = ((int)data[index++] << 24) & 0xFF000000;
				i_camera_mass_y |= ((int)data[index++] << 16) & 0x00FF0000;
				i_camera_mass_y |= ((int)data[index++] << 8) & 0x0000FF00;
				i_camera_mass_y |= ((int)data[index++]) & 0x000000FF;

				//i_robot_x
				i_robot_mass_x = ((int)data[index++] << 24) & 0xFF000000;
				i_robot_mass_x |= ((int)data[index++] << 16) & 0x00FF0000;
				i_robot_mass_x |= ((int)data[index++] << 8) & 0x0000FF00;
				i_robot_mass_x |= ((int)data[index++]) & 0x000000FF;

				//i_robot_y
				i_robot_mass_y = ((int)data[index++] << 24) & 0xFF000000;
				i_robot_mass_y |= ((int)data[index++] << 16) & 0x00FF0000;
				i_robot_mass_y |= ((int)data[index++] << 8) & 0x0000FF00;
				i_robot_mass_y |= ((int)data[index++]) & 0x000000FF;

				//i_circle_rx
				i_circle_rx = ((int)data[index++] << 24) & 0xFF000000;
				i_circle_rx |= ((int)data[index++] << 16) & 0x00FF0000;
				i_circle_rx |= ((int)data[index++] << 8) & 0x0000FF00;
				i_circle_rx |= ((int)data[index++]) & 0x000000FF;

				//i_circle_ry
				i_circle_ry = ((int)data[index++] << 24) & 0xFF000000;
				i_circle_ry |= ((int)data[index++] << 16) & 0x00FF0000;
				i_circle_ry |= ((int)data[index++] << 8) & 0x0000FF00;
				i_circle_ry |= ((int)data[index++]) & 0x000000FF;

				//i_line1_x
				i_line1_x = ((int)data[index++] << 24) & 0xFF000000;
				i_line1_x |= ((int)data[index++] << 16) & 0x00FF0000;
				i_line1_x |= ((int)data[index++] << 8) & 0x0000FF00;
				i_line1_x |= ((int)data[index++]) & 0x000000FF;

				//i_line1_y
				i_line1_y = ((int)data[index++] << 24) & 0xFF000000;
				i_line1_y |= ((int)data[index++] << 16) & 0x00FF0000;
				i_line1_y |= ((int)data[index++] << 8) & 0x0000FF00;
				i_line1_y |= ((int)data[index++]) & 0x000000FF;

				//i_line2_x
				i_line2_x = ((int)data[index++] << 24) & 0xFF000000;
				i_line2_x |= ((int)data[index++] << 16) & 0x00FF0000;
				i_line2_x |= ((int)data[index++] << 8) & 0x0000FF00;
				i_line2_x |= ((int)data[index++]) & 0x000000FF;

				//i_line2_y
				i_line2_y = ((int)data[index++] << 24) & 0xFF000000;
				i_line2_y |= ((int)data[index++] << 16) & 0x00FF0000;
				i_line2_y |= ((int)data[index++] << 8) & 0x0000FF00;
				i_line2_y |= ((int)data[index++]) & 0x000000FF;

				//i_angle
				i_angle = ((int)data[index++] << 24) & 0xFF000000;
				i_angle |= ((int)data[index++] << 16) & 0x00FF0000;
				i_angle |= ((int)data[index++] << 8) & 0x0000FF00;
				i_angle |= ((int)data[index++]) & 0x000000FF;

				//i_type
				i_type = ((int)data[index++] << 24) & 0xFF000000;
				i_type |= ((int)data[index++] << 16) & 0x00FF0000;
				i_type |= ((int)data[index++] << 8) & 0x0000FF00;
				i_type |= ((int)data[index++]) & 0x000000FF;

				//i_score
				i_score = ((int)data[index++] << 24) & 0xFF000000;
				i_score |= ((int)data[index++] << 16) & 0x00FF0000;
				i_score |= ((int)data[index++] << 8) & 0x0000FF00;
				i_score |= ((int)data[index++]) & 0x000000FF;

				//i_pass
				i_pass = ((int)data[index++] << 24) & 0xFF000000;
				i_pass |= ((int)data[index++] << 16) & 0x00FF0000;
				i_pass |= ((int)data[index++] << 8) & 0x0000FF00;
				i_pass |= ((int)data[index++]) & 0x000000FF;

				/*
				//i_index
				i_index = ((int)data[index++] << 24) & 0xFF000000;
				i_index |= ((int)data[index++] << 16) & 0x00FF0000;
				i_index |= ((int)data[index++] << 8) & 0x0000FF00;
				i_index |= ((int)data[index++]) & 0x000000FF;
				*/

				(*out_id)[i] = (float)i_id / (float)scale_factor;
				(*out_cx)[i] = (float)i_camera_x / (float)scale_factor;
				(*out_cy)[i] = (float)i_camera_y / (float)scale_factor;
				(*out_rx)[i] = (float)i_robot_x / (float)scale_factor;
				(*out_ry)[i] = (float)i_robot_y / (float)scale_factor;
				(*out_bound_cx)[i] = (float)i_camera_bound_x / (float)scale_factor;
				(*out_bound_cy)[i] = (float)i_camera_bound_y / (float)scale_factor;
				(*out_bound_rx)[i] = (float)i_robot_bound_x / (float)scale_factor;
				(*out_bound_ry)[i] = (float)i_robot_bound_y / (float)scale_factor;
				(*out_mass_cx)[i] = (float)i_camera_mass_x / (float)scale_factor;
				(*out_mass_cy)[i] = (float)i_camera_mass_y / (float)scale_factor;
				(*out_mass_rx)[i] = (float)i_robot_mass_x / (float)scale_factor;
				(*out_mass_ry)[i] = (float)i_robot_mass_y / (float)scale_factor;
				(*out_circle_rx)[i] = (float)i_circle_rx / (float)scale_factor;
				(*out_circle_ry)[i] = (float)i_circle_ry / (float)scale_factor;
				(*out_line1_x)[i] = (float)i_line1_x / (float)scale_factor;
				(*out_line1_y)[i] = (float)i_line1_y / (float)scale_factor;
				(*out_line2_x)[i] = (float)i_line2_x / (float)scale_factor;
				(*out_line2_y)[i] = (float)i_line2_y / (float)scale_factor;
				(*out_angle)[i] = (float)i_angle / (float)scale_factor;
				(*out_type)[i] = (float)i_type / (float)scale_factor;
				(*out_score)[i] = (float)i_score / (float)scale_factor;
				(*out_pass)[i] = (float)i_pass / (float)scale_factor;
				//(*out_index)[i] = (float)i_index / (float)scale_factor;
			}
		}
	}

	
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	ret = nObject;

    return ret;
}

char* CEyedeaInterface::DB_Get_Mode(int id)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return NULL;
	}

	char command = COMMAND_DB_GET_MODE;

	int len = 4;
	unsigned char* data = new unsigned char[len];

	//index
	data[0] = (id & 0xFF000000) >> 24;
	data[1] = (id & 0x00FF0000) >> 16;
	data[2] = (id & 0x0000FF00) >> 8;
	data[3] = (id & 0x000000FF);

	unsigned int scale_factor = 1;
	m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	char* mode = NULL;

	if (len > 0)
	{
		mode = new char[len];
		memcpy(mode, data, len);
	}

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return mode;
}

int CEyedeaInterface::SetJobName(const int id, const std::string name)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	int ret = 0;

	char command = COMMAND_SET_JOB_NAME;

	int name_size = name.length();

	int len = 4 + name_size;
	unsigned char* data = new unsigned char[len];

	//index
	data[0] = (id & 0xFF000000) >> 24;
	data[1] = (id & 0x00FF0000) >> 16;
	data[2] = (id & 0x0000FF00) >> 8;
	data[3] = (id & 0x000000FF);

	memcpy((data+4), name.c_str(), name_size);

	unsigned int scale_factor = 1;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

std::string CEyedeaInterface::GetJobName(const int id)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return "NoCon";
	}

	char command = COMMAND_GET_JOB_NAME;

	int len = 4 ;
	unsigned char* data = new unsigned char[len];

	//index
	data[0] = (id & 0xFF000000) >> 24;
	data[1] = (id & 0x00FF0000) >> 16;
	data[2] = (id & 0x0000FF00) >> 8;
	data[3] = (id & 0x000000FF);

	unsigned int scale_factor = 1;

	m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	char* name = NULL;

	if (len > 0)
	{
		name = new char[len];
		memcpy(name, data, len);
	}

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	std::string str_name = name;

	if (name != NULL)
	{
		delete name;
		name = NULL;
	}

	return str_name;
}

int CEyedeaInterface::SetToolName(const int id, const std::string name)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	int ret = 0;

	char command = COMMAND_SET_TOOL_NAME;

	int name_size = name.length();

	int len = 4 + name_size;
	unsigned char* data = new unsigned char[len];

	//index
	data[0] = (id & 0xFF000000) >> 24;
	data[1] = (id & 0x00FF0000) >> 16;
	data[2] = (id & 0x0000FF00) >> 8;
	data[3] = (id & 0x000000FF);

	memcpy((data + 4), name.c_str(), name_size);

	unsigned int scale_factor = 1;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

std::string CEyedeaInterface::GetToolName(const int id)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return "NoCon";
	}

	char command = COMMAND_GET_TOOL_NAME;

	int len = 4;
	unsigned char* data = new unsigned char[len];

	//index
	data[0] = (id & 0xFF000000) >> 24;
	data[1] = (id & 0x00FF0000) >> 16;
	data[2] = (id & 0x0000FF00) >> 8;
	data[3] = (id & 0x000000FF);

	unsigned int scale_factor = 1;

	m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	char* name = NULL;

	if (len > 0)
	{
		name = new char[len];
		memcpy(name, data, len);
	}

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	std::string str_name = name;

	if (name != NULL)
	{
		delete name;
		name = NULL;
	}

	return str_name;
}

int CEyedeaInterface::SetToolType(const int id, const int type)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_SET_TOOL_TYPE;

	int len = 4 * 2;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 1;
	int index = 0;

	//id
	data[index++] = (id & 0xFF000000) >> 24;
	data[index++] = (id & 0x00FF0000) >> 16;
	data[index++] = (id & 0x0000FF00) >> 8;
	data[index++] = (id & 0x000000FF);

	data[index++] = (type & 0xFF000000) >> 24;
	data[index++] = (type & 0x00FF0000) >> 16;
	data[index++] = (type & 0x0000FF00) >> 8;
	data[index++] = (type & 0x000000FF);


	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::GetToolType(const int id)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GET_TOOL_TYPE;

	int len = 4;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 1;

	int index = 0;

	//id
	data[index++] = (id & 0xFF000000) >> 24;
	data[index++] = (id & 0x00FF0000) >> 16;
	data[index++] = (id & 0x0000FF00) >> 8;
	data[index++] = (id & 0x000000FF);

	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}

		return ret;
	}

	index = 0;
	int i_type = 0;
	if (len >= 4)
	{
		//i_value
		i_type = ((int)data[index++] << 24) & 0xFF000000;
		i_type |= ((int)data[index++] << 16) & 0x00FF0000;
		i_type |= ((int)data[index++] << 8) & 0x0000FF00;
		i_type |= ((int)data[index++]) & 0x000000FF;
	}

	return i_type;
}

int CEyedeaInterface::SetToolState(const int id, const int state)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_SET_TOOL_STATE;

	int len = 4 * 2;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 1;
	int index = 0;

	//id
	data[index++] = (id & 0xFF000000) >> 24;
	data[index++] = (id & 0x00FF0000) >> 16;
	data[index++] = (id & 0x0000FF00) >> 8;
	data[index++] = (id & 0x000000FF);

	data[index++] = (state & 0xFF000000) >> 24;
	data[index++] = (state & 0x00FF0000) >> 16;
	data[index++] = (state & 0x0000FF00) >> 8;
	data[index++] = (state & 0x000000FF);


	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::GetToolState(const int id)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GET_TOOL_STATE;

	int len = 4;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 1;

	int index = 0;

	//id
	data[index++] = (id & 0xFF000000) >> 24;
	data[index++] = (id & 0x00FF0000) >> 16;
	data[index++] = (id & 0x0000FF00) >> 8;
	data[index++] = (id & 0x000000FF);

	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}

		return ret;
	}

	index = 0;
	int i_state = 0;
	if (len >= 4)
	{
		//i_value
		i_state = ((int)data[index++] << 24) & 0xFF000000;
		i_state |= ((int)data[index++] << 16) & 0x00FF0000;
		i_state |= ((int)data[index++] << 8) & 0x0000FF00;
		i_state |= ((int)data[index++]) & 0x000000FF;
	}

	return i_state;
}

int CEyedeaInterface::SetRobotPose(const int id, double* posj, int posj_size)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_SET_ROBOT_POSE;

	int len = 4 + (posj_size * 4);
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 1000;
	int index = 0;

	//id
	data[index++] = (id & 0xFF000000) >> 24;
	data[index++] = (id & 0x00FF0000) >> 16;
	data[index++] = (id & 0x0000FF00) >> 8;
	data[index++] = (id & 0x000000FF);

	for (int i = 0; i < posj_size; i++)
	{
		int i_posj = posj[i] * scale_factor;
		data[index++] = (i_posj & 0xFF000000) >> 24;
		data[index++] = (i_posj & 0x00FF0000) >> 16;
		data[index++] = (i_posj & 0x0000FF00) >> 8;
		data[index++] = (i_posj & 0x000000FF);
	}

	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

double*  CEyedeaInterface::GetRobotPose(const int id, int* posj_size)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return NULL;
	}

	char command = COMMAND_GET_ROBOT_POSE;

	int len = 4;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 1;

	int index = 0;

	//id
	data[index++] = (id & 0xFF000000) >> 24;
	data[index++] = (id & 0x00FF0000) >> 16;
	data[index++] = (id & 0x0000FF00) >> 8;
	data[index++] = (id & 0x000000FF);

	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}

		return NULL;
	}

	index = 0;

	int data_size = len / 4;
	(*posj_size) = data_size;

	double* out_data = NULL;

	if (data_size > 0)
	{
		if (out_data == NULL)
		{
			out_data = new double[data_size];

			for (int i = 0; i < data_size; i++)
			{
				int i_data = 0;
				//i_value
				i_data = ((int)data[index++] << 24) & 0xFF000000;
				i_data |= ((int)data[index++] << 16) & 0x00FF0000;
				i_data |= ((int)data[index++] << 8) & 0x0000FF00;
				i_data |= ((int)data[index++]) & 0x000000FF;

				double d_data = (double)i_data / (double)scale_factor;

				out_data[i] = d_data;
			}
		}
	}

	return out_data;
}

char* CEyedeaInterface::GetVersion(void)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return NULL;
	}

	char command = COMMAND_GET_VERSION;

	int len = 0;
	unsigned char* data = NULL;

	unsigned int scale_factor = 1;

    m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	char* version = NULL;
	
	if (len > 0)
	{
		version = new char[len];
		memcpy(version, data, len);
	}

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return version;
}

int CEyedeaInterface::Calibration_Save(void)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_CALIBRATION_SAVE;

	int len = 0;
	unsigned char* data = NULL;

	unsigned int scale_factor = 1;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if(ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if(ret == 0){
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::Calibration_Load(void)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_CALIBRATION_LOAD;

	int len = 0;
	unsigned char* data = NULL;

	unsigned int scale_factor = 1;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if(ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while(1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if(ret == 0){
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}

					return ret;
				}
				continue;
			}
		}
	}
	
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::Calibration_Copy(const int id)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_CALIBRATION_COPY;

	int len = 4;
	unsigned char* data = new unsigned char[len];

	//index
	data[0] = (id & 0xFF000000) >> 24;
	data[1] = (id & 0x00FF0000) >> 16;
	data[2] = (id & 0x0000FF00) >> 8;
	data[3] = (id & 0x000000FF);

	unsigned int scale_factor = 1;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

#if 1
int CEyedeaInterface::SetCameraConfig_Default()
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_CAMERA_CONFIG_SET_DEFAULT;

	int len = 0;
	unsigned char* data = NULL; // new char[len];
	unsigned int scale_factor = 1;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::SetCameraConfig_Save()
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_CAMERA_CONFIG_SAVE;

	int len = 0;
	unsigned char* data = NULL; // new char[len];
	unsigned int scale_factor = 1;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::SetCameraConfig_Load()
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_CAMERA_CONFIG_LOAD;
	unsigned char* data = NULL; // new char[len];
	int len = 0;

	unsigned int scale_factor = 1;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}

					return ret;
				}
				continue;
			}
		}
	}

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::GetCameraConfig(int type)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_CAMERA_CONFIG_GET;

	int len = 4;
	unsigned char* data = new unsigned char[len];

	//type
	data[0] = (type & 0xFF000000) >> 24;
	data[1] = (type & 0x00FF0000) >> 16;
	data[2] = (type & 0x0000FF00) >> 8;
	data[3] = (type & 0x000000FF);

	unsigned int scale_factor = 1;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, (unsigned char **)&data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, (unsigned char **)&data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}
					return ret;
				}
				continue;
			}
		}
	}
	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}

		return ret;
	}

	ret = 0;

	if (len >= 4)
	{
		//x
		int val = ((int)data[0] << 24) & 0xFF000000;
		val |= ((int)data[1] << 16) & 0x00FF0000;
		val |= ((int)data[2] << 8) & 0x0000FF00;
		val |= ((int)data[3]) & 0x000000FF;
		ret = val;
	}
	//	printf("command val ( 0x%x / 0x%x )\n", type, *retval);
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}
#endif

int CEyedeaInterface::SetCameraConfig(int type, int value, int value2)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	//char command = COMMAND_SET_CAMERA_CONFIG;
	char command = COMMAND_CAMERA_CONFIG_SET;

	int len = 12;
	unsigned char* data = new unsigned char[len];

	//type
	data[0] = (type & 0xFF000000) >> 24;
	data[1] = (type & 0x00FF0000) >> 16;
	data[2] = (type & 0x0000FF00) >> 8;
	data[3] = (type & 0x000000FF);

	//value
	data[4] = (value & 0xFF000000) >> 24;
	data[5] = (value & 0x00FF0000) >> 16;
	data[6] = (value & 0x0000FF00) >> 8;
	data[7] = (value & 0x000000FF);

	//value
	data[8] = (value2 & 0xFF000000) >> 24;
	data[9] = (value2 & 0x00FF0000) >> 16;
	data[10] = (value2 & 0x0000FF00) >> 8;
	data[11] = (value2 & 0x000000FF);


	unsigned int scale_factor = 1;
	int ret = 0;
    ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	
	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

    return ret;
}

int CEyedeaInterface::BackgroundLearning(void)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_BACKGROUND_LEARN;

	int len = 0;
	unsigned char* data = NULL;

	unsigned int scale_factor = 1;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::Geometry_MeetPoint(const int base_id, const int target_id, float *out_value_x, float *out_value_y)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GEOMETRY_GET_MEET_POINT;

	int len = 4 * 2;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 10000;

	int index = 0;

	//base_id
	data[index++] = (base_id & 0xFF000000) >> 24;
	data[index++] = (base_id & 0x00FF0000) >> 16;
	data[index++] = (base_id & 0x0000FF00) >> 8;
	data[index++] = (base_id & 0x000000FF);

	//target_id
	data[index++] = (target_id & 0xFF000000) >> 24;
	data[index++] = (target_id & 0x00FF0000) >> 16;
	data[index++] = (target_id & 0x0000FF00) >> 8;
	data[index++] = (target_id & 0x000000FF);

	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}

		return ret;
	}

	int i_value_x = 0;
	int i_value_y = 0;

	if (len >= 8)
	{
		//i_value
		i_value_x = ((int)data[0] << 24) & 0xFF000000;
		i_value_x |= ((int)data[1] << 16) & 0x00FF0000;
		i_value_x |= ((int)data[2] << 8) & 0x0000FF00;
		i_value_x |= ((int)data[3]) & 0x000000FF;

		i_value_y = ((int)data[4] << 24) & 0xFF000000;
		i_value_y |= ((int)data[5] << 16) & 0x00FF0000;
		i_value_y |= ((int)data[6] << 8) & 0x0000FF00;
		i_value_y |= ((int)data[7]) & 0x000000FF;
	}

	(*out_value_x) = (float)i_value_x / (float)scale_factor;
	(*out_value_y) = (float)i_value_y / (float)scale_factor;

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::Geometry_Get_Distance(const int base_id, const int target_id, float *out_value)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GEOMETRY_GET_DISTANCE;

	int len = 4 * 2;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 10000;

	int index = 0;

	//base_id
	data[index++] = (base_id & 0xFF000000) >> 24;
	data[index++] = (base_id & 0x00FF0000) >> 16;
	data[index++] = (base_id & 0x0000FF00) >> 8;
	data[index++] = (base_id & 0x000000FF);

	//target_id
	data[index++] = (target_id & 0xFF000000) >> 24;
	data[index++] = (target_id & 0x00FF0000) >> 16;
	data[index++] = (target_id & 0x0000FF00) >> 8;
	data[index++] = (target_id & 0x000000FF);


	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}

		return ret;
	}

	int i_value = 0;
	if (len >= 4)
	{
		//i_value
		i_value = ((int)data[0] << 24) & 0xFF000000;
		i_value |= ((int)data[1] << 16) & 0x00FF0000;
		i_value |= ((int)data[2] << 8) & 0x0000FF00;
		i_value |= ((int)data[3]) & 0x000000FF;
	}

	(*out_value) = (float)i_value / (float)scale_factor;

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::Geometry_Get_Angle(const int base_id, const int target_id, float *out_value)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GEOMETRY_GET_ANGLE;

	int len = 4 * 2;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 10000;

	int index = 0;

	//base_id
	data[index++] = (base_id & 0xFF000000) >> 24;
	data[index++] = (base_id & 0x00FF0000) >> 16;
	data[index++] = (base_id & 0x0000FF00) >> 8;
	data[index++] = (base_id & 0x000000FF);

	//target_id
	data[index++] = (target_id & 0xFF000000) >> 24;
	data[index++] = (target_id & 0x00FF0000) >> 16;
	data[index++] = (target_id & 0x0000FF00) >> 8;
	data[index++] = (target_id & 0x000000FF);


	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}

		return ret;
	}

	int i_value = 0;
	if (len >= 4)
	{
		//i_value
		i_value = ((int)data[0] << 24) & 0xFF000000;
		i_value |= ((int)data[1] << 16) & 0x00FF0000;
		i_value |= ((int)data[2] << 8) & 0x0000FF00;
		i_value |= ((int)data[3]) & 0x000000FF;
	}

	(*out_value) = (float)i_value / (float)scale_factor;

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::Geometry_Distance(const int base_id, const int target_id, float *out_value, int *out_pass)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GEOMETRY_DISTANCE;

	int len = 4 * 4;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 10000;

	int index = 0;

	//base_id
	data[index++] = (base_id & 0xFF000000) >> 24;
	data[index++] = (base_id & 0x00FF0000) >> 16;
	data[index++] = (base_id & 0x0000FF00) >> 8;
	data[index++] = (base_id & 0x000000FF);

	//target_id
	data[index++] = (target_id & 0xFF000000) >> 24;
	data[index++] = (target_id & 0x00FF0000) >> 16;
	data[index++] = (target_id & 0x0000FF00) >> 8;
	data[index++] = (target_id & 0x000000FF);
	
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}

		return ret;
	}

	int i_value = 0;
	int i_pass = 0;

	if (len >= 8)
	{
		//i_value
		i_value = ((int)data[0] << 24) & 0xFF000000;
		i_value |= ((int)data[1] << 16) & 0x00FF0000;
		i_value |= ((int)data[2] << 8) & 0x0000FF00;
		i_value |= ((int)data[3]) & 0x000000FF;

		//i_pass
		i_pass = ((int)data[4] << 24) & 0xFF000000;
		i_pass |= ((int)data[5] << 16) & 0x00FF0000;
		i_pass |= ((int)data[6] << 8) & 0x0000FF00;
		i_pass |= ((int)data[7]) & 0x000000FF;
	}

	(*out_value) = (float)i_value / (float)scale_factor;
	(*out_pass) = i_pass;

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::Geometry_Set_Inspection_Distance(const int base_id, const int target_id, const float value)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GEOMETRY_SET_INSPECTION_DISTANCE;

	int len = 4 * 3;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 10000;

	int index = 0;

	//base_id
	data[index++] = (base_id & 0xFF000000) >> 24;
	data[index++] = (base_id & 0x00FF0000) >> 16;
	data[index++] = (base_id & 0x0000FF00) >> 8;
	data[index++] = (base_id & 0x000000FF);

	//target_id
	data[index++] = (target_id & 0xFF000000) >> 24;
	data[index++] = (target_id & 0x00FF0000) >> 16;
	data[index++] = (target_id & 0x0000FF00) >> 8;
	data[index++] = (target_id & 0x000000FF);

	//value
	int i_value = value * scale_factor;
	data[index++] = (i_value & 0xFF000000) >> 24;
	data[index++] = (i_value & 0x00FF0000) >> 16;
	data[index++] = (i_value & 0x0000FF00) >> 8;
	data[index++] = (i_value & 0x000000FF);


	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::Geometry_Get_Inspection_Distance(const int base_id, const int target_id, float *out_value)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GEOMETRY_GET_INSPECTION_DISTANCE;

	int len = 4 * 2;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 10000;

	int index = 0;

	//base_id
	data[index++] = (base_id & 0xFF000000) >> 24;
	data[index++] = (base_id & 0x00FF0000) >> 16;
	data[index++] = (base_id & 0x0000FF00) >> 8;
	data[index++] = (base_id & 0x000000FF);

	//target_id
	data[index++] = (target_id & 0xFF000000) >> 24;
	data[index++] = (target_id & 0x00FF0000) >> 16;
	data[index++] = (target_id & 0x0000FF00) >> 8;
	data[index++] = (target_id & 0x000000FF);


	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}

		return ret;
	}

	int i_value = 0;
	if (len >= 4)
	{
		//i_value
		i_value = ((int)data[0] << 24) & 0xFF000000;
		i_value |= ((int)data[1] << 16) & 0x00FF0000;
		i_value |= ((int)data[2] << 8) & 0x0000FF00;
		i_value |= ((int)data[3]) & 0x000000FF;
	}

	(*out_value) = (float)i_value / (float)scale_factor;

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::Geometry_Set_Inspection_Distance_Tolerance_Rate(const int base_id, const int target_id, const float value)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GEOMETRY_SET_INSPECTION_DISTANCE_TOL_RATE;

	int len = 4 * 3;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 10000;

	int index = 0;

	//base_id
	data[index++] = (base_id & 0xFF000000) >> 24;
	data[index++] = (base_id & 0x00FF0000) >> 16;
	data[index++] = (base_id & 0x0000FF00) >> 8;
	data[index++] = (base_id & 0x000000FF);

	//target_id
	data[index++] = (target_id & 0xFF000000) >> 24;
	data[index++] = (target_id & 0x00FF0000) >> 16;
	data[index++] = (target_id & 0x0000FF00) >> 8;
	data[index++] = (target_id & 0x000000FF);

	//value
	int i_value = value * scale_factor;
	data[index++] = (i_value & 0xFF000000) >> 24;
	data[index++] = (i_value & 0x00FF0000) >> 16;
	data[index++] = (i_value & 0x0000FF00) >> 8;
	data[index++] = (i_value & 0x000000FF);


	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::Geometry_Get_Inspection_Distance_Tolerance_Rate(const int base_id, const int target_id, float *out_value)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GEOMETRY_GET_INSPECTION_DISTANCE_TOL_RATE;

	int len = 4 * 2;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 10000;

	int index = 0;

	//base_id
	data[index++] = (base_id & 0xFF000000) >> 24;
	data[index++] = (base_id & 0x00FF0000) >> 16;
	data[index++] = (base_id & 0x0000FF00) >> 8;
	data[index++] = (base_id & 0x000000FF);

	//target_id
	data[index++] = (target_id & 0xFF000000) >> 24;
	data[index++] = (target_id & 0x00FF0000) >> 16;
	data[index++] = (target_id & 0x0000FF00) >> 8;
	data[index++] = (target_id & 0x000000FF);


	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}

		return ret;
	}

	int i_value = 0;
	if (len >= 4)
	{
		//i_value
		i_value = ((int)data[0] << 24) & 0xFF000000;
		i_value |= ((int)data[1] << 16) & 0x00FF0000;
		i_value |= ((int)data[2] << 8) & 0x0000FF00;
		i_value |= ((int)data[3]) & 0x000000FF;
	}

	(*out_value) = (float)i_value / (float)scale_factor;

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::Geometry_Set_Inspection_Angle(const int base_id, const int target_id, const float value)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GEOMETRY_SET_INSPECTION_ANGLE;

	int len = 4 * 3;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 10000;

	int index = 0;

	//base_id
	data[index++] = (base_id & 0xFF000000) >> 24;
	data[index++] = (base_id & 0x00FF0000) >> 16;
	data[index++] = (base_id & 0x0000FF00) >> 8;
	data[index++] = (base_id & 0x000000FF);

	//target_id
	data[index++] = (target_id & 0xFF000000) >> 24;
	data[index++] = (target_id & 0x00FF0000) >> 16;
	data[index++] = (target_id & 0x0000FF00) >> 8;
	data[index++] = (target_id & 0x000000FF);

	//value
	int i_value = value * scale_factor;
	data[index++] = (i_value & 0xFF000000) >> 24;
	data[index++] = (i_value & 0x00FF0000) >> 16;
	data[index++] = (i_value & 0x0000FF00) >> 8;
	data[index++] = (i_value & 0x000000FF);


	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::Geometry_Get_Inspection_Angle(const int base_id, const int target_id, float *out_value)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GEOMETRY_GET_INSPECTION_ANGLE;

	int len = 4 * 2;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 10000;

	int index = 0;

	//base_id
	data[index++] = (base_id & 0xFF000000) >> 24;
	data[index++] = (base_id & 0x00FF0000) >> 16;
	data[index++] = (base_id & 0x0000FF00) >> 8;
	data[index++] = (base_id & 0x000000FF);

	//target_id
	data[index++] = (target_id & 0xFF000000) >> 24;
	data[index++] = (target_id & 0x00FF0000) >> 16;
	data[index++] = (target_id & 0x0000FF00) >> 8;
	data[index++] = (target_id & 0x000000FF);


	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}

		return ret;
	}

	int i_value = 0;
	if (len >= 4)
	{
		//i_value
		i_value = ((int)data[0] << 24) & 0xFF000000;
		i_value |= ((int)data[1] << 16) & 0x00FF0000;
		i_value |= ((int)data[2] << 8) & 0x0000FF00;
		i_value |= ((int)data[3]) & 0x000000FF;
	}

	(*out_value) = (float)i_value / (float)scale_factor;

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::Geometry_Set_Inspection_Angle_Tolerance_Rate(const int base_id, const int target_id, const float value)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GEOMETRY_SET_INSPECTION_ANGLE_TOL_RATE;

	int len = 4 * 3;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 10000;

	int index = 0;

	//base_id
	data[index++] = (base_id & 0xFF000000) >> 24;
	data[index++] = (base_id & 0x00FF0000) >> 16;
	data[index++] = (base_id & 0x0000FF00) >> 8;
	data[index++] = (base_id & 0x000000FF);

	//target_id
	data[index++] = (target_id & 0xFF000000) >> 24;
	data[index++] = (target_id & 0x00FF0000) >> 16;
	data[index++] = (target_id & 0x0000FF00) >> 8;
	data[index++] = (target_id & 0x000000FF);

	//value
	int i_value = value * scale_factor;
	data[index++] = (i_value & 0xFF000000) >> 24;
	data[index++] = (i_value & 0x00FF0000) >> 16;
	data[index++] = (i_value & 0x0000FF00) >> 8;
	data[index++] = (i_value & 0x000000FF);


	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::Geometry_Get_Inspection_Angle_Tolerance_Rate(const int base_id, const int target_id, float *out_value)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GEOMETRY_GET_INSPECTION_ANGLE_TOL_RATE;

	int len = 4 * 2;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 10000;

	int index = 0;

	//base_id
	data[index++] = (base_id & 0xFF000000) >> 24;
	data[index++] = (base_id & 0x00FF0000) >> 16;
	data[index++] = (base_id & 0x0000FF00) >> 8;
	data[index++] = (base_id & 0x000000FF);

	//target_id
	data[index++] = (target_id & 0xFF000000) >> 24;
	data[index++] = (target_id & 0x00FF0000) >> 16;
	data[index++] = (target_id & 0x0000FF00) >> 8;
	data[index++] = (target_id & 0x000000FF);


	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}

		return ret;
	}

	int i_value = 0;
	if (len >= 4)
	{
		//i_value
		i_value = ((int)data[0] << 24) & 0xFF000000;
		i_value |= ((int)data[1] << 16) & 0x00FF0000;
		i_value |= ((int)data[2] << 8) & 0x0000FF00;
		i_value |= ((int)data[3]) & 0x000000FF;
	}

	(*out_value) = (float)i_value / (float)scale_factor;

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::Geometry_Angle(const int base_id, const int target_id, float *out_value, int *out_pass)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GEOMETRY_ANGLE;

	int len = 4 * 2;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 10000;

	int index = 0;

	//base_id
	data[index++] = (base_id & 0xFF000000) >> 24;
	data[index++] = (base_id & 0x00FF0000) >> 16;
	data[index++] = (base_id & 0x0000FF00) >> 8;
	data[index++] = (base_id & 0x000000FF);

	//target_id
	data[index++] = (target_id & 0xFF000000) >> 24;
	data[index++] = (target_id & 0x00FF0000) >> 16;
	data[index++] = (target_id & 0x0000FF00) >> 8;
	data[index++] = (target_id & 0x000000FF);

	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}

		return ret;
	}

	int i_value = 0;
	int i_pass = 0;

	if (len >= 8)
	{
		//i_value
		i_value = ((int)data[0] << 24) & 0xFF000000;
		i_value |= ((int)data[1] << 16) & 0x00FF0000;
		i_value |= ((int)data[2] << 8) & 0x0000FF00;
		i_value |= ((int)data[3]) & 0x000000FF;

		i_pass = ((int)data[4] << 24) & 0xFF000000;
		i_pass |= ((int)data[5] << 16) & 0x00FF0000;
		i_pass |= ((int)data[6] << 8) & 0x0000FF00;
		i_pass |= ((int)data[7]) & 0x000000FF;
	}

	(*out_value) = (float)i_value / (float)scale_factor;
	(*out_pass) = i_pass;

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::Geometry_Clear(void)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_GEOMETRY_CLEAR;

	int len = 0;
	unsigned char* data = new unsigned char[len];
	unsigned int scale_factor = 1;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::Histogram_Set_Range(const int id, const int option, const int min_value, const int max_value)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_HISTOGRAM_SET_RANGE;

	int len = 4 * 4;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 10000;

	int index = 0;

	//id
	data[index++] = (id & 0xFF000000) >> 24;
	data[index++] = (id & 0x00FF0000) >> 16;
	data[index++] = (id & 0x0000FF00) >> 8;
	data[index++] = (id & 0x000000FF);

	//option
	data[index++] = (option & 0xFF000000) >> 24;
	data[index++] = (option & 0x00FF0000) >> 16;
	data[index++] = (option & 0x0000FF00) >> 8;
	data[index++] = (option & 0x000000FF);

	//min value
	data[index++] = (min_value & 0xFF000000) >> 24;
	data[index++] = (min_value & 0x00FF0000) >> 16;
	data[index++] = (min_value & 0x0000FF00) >> 8;
	data[index++] = (min_value & 0x000000FF);

	//max value
	data[index++] = (max_value & 0xFF000000) >> 24;
	data[index++] = (max_value & 0x00FF0000) >> 16;
	data[index++] = (max_value & 0x0000FF00) >> 8;
	data[index++] = (max_value & 0x000000FF);

	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::Histogram_Get_Range(const int id, const int option, int *out_min_value, int *out_max_value)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_HISTOGRAM_GET_RANGE;

	int len = 4 * 2;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 10000;

	int index = 0;

	//id
	data[index++] = (id & 0xFF000000) >> 24;
	data[index++] = (id & 0x00FF0000) >> 16;
	data[index++] = (id & 0x0000FF00) >> 8;
	data[index++] = (id & 0x000000FF);

	//option
	data[index++] = (option & 0xFF000000) >> 24;
	data[index++] = (option & 0x00FF0000) >> 16;
	data[index++] = (option & 0x0000FF00) >> 8;
	data[index++] = (option & 0x000000FF);


	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}

		return ret;
	}

	index = 0;
	int i_min_value = -1;
	int i_max_value = -1;
	if (len >= 8)
	{
		//i_value
		i_min_value = ((int)data[index++] << 24) & 0xFF000000;
		i_min_value |= ((int)data[index++] << 16) & 0x00FF0000;
		i_min_value |= ((int)data[index++] << 8) & 0x0000FF00;
		i_min_value |= ((int)data[index++]) & 0x000000FF;

		//i_value
		i_max_value = ((int)data[index++] << 24) & 0xFF000000;
		i_max_value |= ((int)data[index++] << 16) & 0x00FF0000;
		i_max_value |= ((int)data[index++] << 8) & 0x0000FF00;
		i_max_value |= ((int)data[index++]) & 0x000000FF;
	}

	(*out_min_value) = i_min_value;
	(*out_max_value) = i_max_value;

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::Histogram_Get_Graph(const int id, float** out_histogram, float** out_histogram_b, float** out_histogram_g, float** out_histogram_r, float* out_histogram_size)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_HISTOGRAM_GET_GRAPH;

	int len = 4;
	unsigned char* data = new unsigned char[len];

	//index
	int data_index = 0;
	data[data_index++] = (id & 0xFF000000) >> 24;
	data[data_index++] = (id & 0x00FF0000) >> 16;
	data[data_index++] = (id & 0x0000FF00) >> 8;
	data[data_index++] = (id & 0x000000FF);

	unsigned int scale_factor = 1;
	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
	if (ret == EYEDEA_ERROR_INVALID_MEMORY)
	{
		int sec = 0;
		while (1)
		{
			ret = m_cls_eth_client->Open(m_ip, m_port);
			if (ret == 0) {
				ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);
				break;
			}
			else
			{
				boost::this_thread::sleep(boost::posix_time::millisec(1000));  //1 msec sleep
				sec++;
				if (sec >= 60)
				{
					if (data != NULL)
					{
						delete data;
						data = NULL;
					}

					return ret;
				}
				continue;
			}
		}
	}
	
	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}

		return ret;
	}

	int i_histogram_size = 0;
	int i_histogram_data = 0;

	int index = 0;

	//printf("len=%d\n", len);

	if (len >= 4)
	{
		//i_histogram_size
		i_histogram_size = ((int)data[index++] << 24) & 0xFF000000;
		i_histogram_size |= ((int)data[index++] << 16) & 0x00FF0000;
		i_histogram_size |= ((int)data[index++] << 8) & 0x0000FF00;
		i_histogram_size |= ((int)data[index++]) & 0x000000FF;

		printf("i_histogram_size=%d, scale_factor=%d\n", i_histogram_size, scale_factor);

		(*out_histogram_size) = (float)i_histogram_size / (float)scale_factor;

		int histogram_size = (int)(*out_histogram_size);
		if (histogram_size > 0)
		{
			//gray
			if ((*out_histogram) != NULL)	free((*out_histogram));
			(*out_histogram) = (float *)malloc(sizeof(float)*histogram_size);

			for (int i = 0; i < histogram_size; i++)
			{
				//i_score
				i_histogram_data = ((int)data[index++] << 24) & 0xFF000000;
				i_histogram_data |= ((int)data[index++] << 16) & 0x00FF0000;
				i_histogram_data |= ((int)data[index++] << 8) & 0x0000FF00;
				i_histogram_data |= ((int)data[index++]) & 0x000000FF;

				(*out_histogram)[i] = (float)i_histogram_data / (float)scale_factor;
			}

			//b
			if ((*out_histogram_b) != NULL)	free((*out_histogram_b));
			(*out_histogram_b) = (float *)malloc(sizeof(float)*histogram_size);

			for (int i = 0; i < histogram_size; i++)
			{
				//i_score
				i_histogram_data = ((int)data[index++] << 24) & 0xFF000000;
				i_histogram_data |= ((int)data[index++] << 16) & 0x00FF0000;
				i_histogram_data |= ((int)data[index++] << 8) & 0x0000FF00;
				i_histogram_data |= ((int)data[index++]) & 0x000000FF;

				(*out_histogram_b)[i] = (float)i_histogram_data / (float)scale_factor;
			}

			//g
			if ((*out_histogram_g) != NULL)	free((*out_histogram_g));
			(*out_histogram_g) = (float *)malloc(sizeof(float)*histogram_size);

			for (int i = 0; i < histogram_size; i++)
			{
				//i_score
				i_histogram_data = ((int)data[index++] << 24) & 0xFF000000;
				i_histogram_data |= ((int)data[index++] << 16) & 0x00FF0000;
				i_histogram_data |= ((int)data[index++] << 8) & 0x0000FF00;
				i_histogram_data |= ((int)data[index++]) & 0x000000FF;

				(*out_histogram_g)[i] = (float)i_histogram_data / (float)scale_factor;
			}

			//r
			if ((*out_histogram_r) != NULL)	free((*out_histogram_r));
			(*out_histogram_r) = (float *)malloc(sizeof(float)*histogram_size);

			for (int i = 0; i < histogram_size; i++)
			{
				//i_score
				i_histogram_data = ((int)data[index++] << 24) & 0xFF000000;
				i_histogram_data |= ((int)data[index++] << 16) & 0x00FF0000;
				i_histogram_data |= ((int)data[index++] << 8) & 0x0000FF00;
				i_histogram_data |= ((int)data[index++]) & 0x000000FF;

				(*out_histogram_r)[i] = (float)i_histogram_data / (float)scale_factor;
			}
		}
	}


	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return i_histogram_size;
}

int CEyedeaInterface::Histogram_Set_Use_Element(const int id, const int option)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_HISTOGRAM_SET_USE_ELEMENT;

	int len = 4 * 2;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 10000;

	int index = 0;

	//id
	data[index++] = (id & 0xFF000000) >> 24;
	data[index++] = (id & 0x00FF0000) >> 16;
	data[index++] = (id & 0x0000FF00) >> 8;
	data[index++] = (id & 0x000000FF);

	//option
	data[index++] = (option & 0xFF000000) >> 24;
	data[index++] = (option & 0x00FF0000) >> 16;
	data[index++] = (option & 0x0000FF00) >> 8;
	data[index++] = (option & 0x000000FF);


	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::Histogram_Get_Use_Element(const int id, int *out_option)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_HISTOGRAM_GET_USE_ELEMENT;

	int len = 4;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 10000;

	int index = 0;

	//id
	data[index++] = (id & 0xFF000000) >> 24;
	data[index++] = (id & 0x00FF0000) >> 16;
	data[index++] = (id & 0x0000FF00) >> 8;
	data[index++] = (id & 0x000000FF);


	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}

		return ret;
	}

	index = 0;
	int i_option = 0;
	if (len >= 4)
	{
		//i_value
		i_option = ((int)data[index++] << 24) & 0xFF000000;
		i_option |= ((int)data[index++] << 16) & 0x00FF0000;
		i_option |= ((int)data[index++] << 8) & 0x0000FF00;
		i_option |= ((int)data[index++]) & 0x000000FF;
	}

	(*out_option) = i_option;

	return ret;
}

int CEyedeaInterface::Histogram_Get_Pixel_Count(const int index1, const int index2, int *out_count)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_HISTOGRAM_GET_PIXEL_COUNT_RESULT;

	int len = 4*2;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 10000;

	int index = 0;

	//id
	data[index++] = (index1 & 0xFF000000) >> 24;
	data[index++] = (index1 & 0x00FF0000) >> 16;
	data[index++] = (index1 & 0x0000FF00) >> 8;
	data[index++] = (index1 & 0x000000FF);

	data[index++] = (index2 & 0xFF000000) >> 24;
	data[index++] = (index2 & 0x00FF0000) >> 16;
	data[index++] = (index2 & 0x0000FF00) >> 8;
	data[index++] = (index2 & 0x000000FF);


	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}

		return ret;
	}

	index = 0;
	int i_count = 0;
	if (len >= 4)
	{
		//i_value
		i_count = ((int)data[index++] << 24) & 0xFF000000;
		i_count |= ((int)data[index++] << 16) & 0x00FF0000;
		i_count |= ((int)data[index++] << 8) & 0x0000FF00;
		i_count |= ((int)data[index++]) & 0x000000FF;
	}

	(*out_count) = i_count;

	return ret;
}

int CEyedeaInterface::Histogram_Get_Pixel_Count(const int id, int *out_count)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_HISTOGRAM_GET_PIXEL_COUNT_ID;

	int len = 4 * 2;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 10000;

	int index = 0;

	//id
	data[index++] = (id & 0xFF000000) >> 24;
	data[index++] = (id & 0x00FF0000) >> 16;
	data[index++] = (id & 0x0000FF00) >> 8;
	data[index++] = (id & 0x000000FF);

	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}

		return ret;
	}

	index = 0;
	int i_count = 0;
	if (len >= 4)
	{
		//i_value
		i_count = ((int)data[index++] << 24) & 0xFF000000;
		i_count |= ((int)data[index++] << 16) & 0x00FF0000;
		i_count |= ((int)data[index++] << 8) & 0x0000FF00;
		i_count |= ((int)data[index++]) & 0x000000FF;
	}

	(*out_count) = i_count;

	return ret;
}

int CEyedeaInterface::Histogram_Set_Inspection_Pixel_Count(const int id, const int count)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_HISTOGRAM_SET_INSPECTION_PIXEL_COUNT;

	int len = 4 * 2;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 10000;

	int index = 0;

	//id
	data[index++] = (id & 0xFF000000) >> 24;
	data[index++] = (id & 0x00FF0000) >> 16;
	data[index++] = (id & 0x0000FF00) >> 8;
	data[index++] = (id & 0x000000FF);

	data[index++] = (count & 0xFF000000) >> 24;
	data[index++] = (count & 0x00FF0000) >> 16;
	data[index++] = (count & 0x0000FF00) >> 8;
	data[index++] = (count & 0x000000FF);


	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::Histogram_Get_Inspection_Pixel_Count(const int id, int *out_count)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_HISTOGRAM_GET_INSPECTION_PIXEL_COUNT;

	int len = 4;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 10000;

	int index = 0;

	//id
	data[index++] = (id & 0xFF000000) >> 24;
	data[index++] = (id & 0x00FF0000) >> 16;
	data[index++] = (id & 0x0000FF00) >> 8;
	data[index++] = (id & 0x000000FF);

	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}

		return ret;
	}

	index = 0;
	int i_count = 0;
	if (len >= 4)
	{
		//i_value
		i_count = ((int)data[index++] << 24) & 0xFF000000;
		i_count |= ((int)data[index++] << 16) & 0x00FF0000;
		i_count |= ((int)data[index++] << 8) & 0x0000FF00;
		i_count |= ((int)data[index++]) & 0x000000FF;
	}

	(*out_count) = i_count;

	return ret;
}

int CEyedeaInterface::Histogram_Set_Inspection_Pixel_Count_Tolerance_Rate(const int id, const float rate)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_HISTOGRAM_SET_INSPECTION_PIXEL_COUNT_TOL_RATE;

	int len = 4 * 2;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 10000;

	int i_rate = rate * scale_factor;

	int index = 0;

	//id
	data[index++] = (id & 0xFF000000) >> 24;
	data[index++] = (id & 0x00FF0000) >> 16;
	data[index++] = (id & 0x0000FF00) >> 8;
	data[index++] = (id & 0x000000FF);

	data[index++] = (i_rate & 0xFF000000) >> 24;
	data[index++] = (i_rate & 0x00FF0000) >> 16;
	data[index++] = (i_rate & 0x0000FF00) >> 8;
	data[index++] = (i_rate & 0x000000FF);


	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (data != NULL)
	{
		delete data;
		data = NULL;
	}

	return ret;
}

int CEyedeaInterface::Histogram_Get_Inspection_Pixel_Count_Tolerance_Rate(const int id, float *out_rate)
{
	boost::unique_lock<boost::mutex> scoped_lock(mutex);

	if (m_cls_eth_client == NULL)
	{
		printf("Before accessing the ERVS\n");
		return EYEDEA_ERROR_INVALID_MEMORY;
	}

	char command = COMMAND_HISTOGRAM_GET_INSPECTION_PIXEL_COUNT_TOL_RATE;

	int len = 4;
	unsigned char* data = new unsigned char[len];

	unsigned int scale_factor = 10000;

	int index = 0;

	//id
	data[index++] = (id & 0xFF000000) >> 24;
	data[index++] = (id & 0x00FF0000) >> 16;
	data[index++] = (id & 0x0000FF00) >> 8;
	data[index++] = (id & 0x000000FF);

	int ret = 0;
	ret = m_cls_eth_client->Send(command, &scale_factor, &data, &len);

	if (ret != 0)
	{
		if (data != NULL)
		{
			delete data;
			data = NULL;
		}

		return ret;
	}

	index = 0;
	int i_count = 0;
	if (len >= 4)
	{
		//i_value
		i_count = ((int)data[index++] << 24) & 0xFF000000;
		i_count |= ((int)data[index++] << 16) & 0x00FF0000;
		i_count |= ((int)data[index++] << 8) & 0x0000FF00;
		i_count |= ((int)data[index++]) & 0x000000FF;
}

	(*out_rate) = (float)i_count / (float)scale_factor;

	return ret;
}

#if 0
int CEyedeaInterface::ThreadFunctionNetwork(void)
{
	int length;
	char* recv_data;
	char command = 0;

	while (m_run_thread_net)												//thread loop
	{
		//printf("\033[2J");
		//printf("\033[1;1H");

		char data = COMMAND_GET_IMAGE;
		recv_data = m_cls_eth_client->Send(data, &length);

#if 0
		if (data == COMMAND_GET_IMAGE)
		{
			{
				boost::lock_guard<boost::recursive_mutex> lock(m_guard);

				if (m_display.empty())
				{
					m_display = cv::Mat::zeros(cv::Size(640, 480), CV_8UC3); //cv::imread("base.png");		//opencv mat for display
				}

				memcpy(m_display.data, recv_data, length);

				delete (recv_data);
				recv_data = NULL;
			}
		}
#endif

#if 0
		if (*recv_data == (unsigned char)'n')
			cout << "wrong data" << endl;
		else
			cout << "data : " << recv_data << " data_length : " << length << endl;
#endif

		boost::this_thread::sleep(boost::posix_time::millisec(1000));		//1 msec sleep
	};
}
#endif
