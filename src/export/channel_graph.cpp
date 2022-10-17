/**
 * @file channel_graph.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Implementation of Channel_Graph Class
 * @version 0.1.0
 * @date 2022-10-17
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 * 
 */

#include "export/channel_graph.h"

bool Channel_Graph::addChannel(const std::string& id, const std::string& from_, const std::string& to_) {
    unsigned from_i = nodeIndex(from_);
    unsigned to_i = nodeIndex(to_);
    if (from_i == MAX_INDEX || to_i == MAX_INDEX) return false;
    from.push_back(from_i);
    to.push_back(to_i);
    channels.push_back(id);
    return true;
}

unsigned Channel_Graph::nodeIndex(const std::string& id) {
    for (unsigned i = 0; i != nodes.size(); ++i) {
        if (nodes[i] == id) return i;
    }
    return MAX_INDEX;
}

unsigned Channel_Graph::channelIndex(const std::string& id) {
    for (unsigned i = 0; i != channels.size(); ++i) {
        if (channels[i] == id) return i;
    }
    return MAX_INDEX;
}
