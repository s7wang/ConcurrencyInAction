#include <iostream>
#include <thread>
#include <mutex>
#include <future>

class connection_set;
bool done(connection_set& connections);

void process_connections(connection_set& connections)
{
    while (!done(connections))
    {
        for (onnection_siterator
                connection = connections.begin(), end = connections.end();
                connection != end; 
                connection++)
        {
            if(connection->has_incoming_data())
            {
                data_packet data = connection->incoming();
                std::promise<payload_type>& p = connection->get_promise(data.id);
                p.set_value(data.payload);
            }
            if(connection->has_outgoing_data())
            {
                outgoing_packet_data data = 
                    connection->top_of_outgoing_queue();
                connection->send(data.payload);
                data.promise.set_value(ture);
            }
        }
    }
}

int main(int argc, const char** argv) {

    return 0;
}