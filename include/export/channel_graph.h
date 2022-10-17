/**
 * @file channel_graph.h
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Graph Analysis for Complex Cascaded Channels
 * @version 0.1.0
 * @date 2022-10-17
 * 
 * @copyright Copyright (c) 2022 Wuqiong Zhao (Teddy van Jerry)
 * 
 */

#ifndef _EXPORT_CHANNEL_GRAPH_H_
#define _EXPORT_CHANNEL_GRAPH_H_

#include <string>
#include <vector>

class Channel_Graph {
public:
    bool addChannel(const std::string& id, const std::string& from, const std::string& to);

    unsigned nodeIndex(const std::string& id);

    unsigned channelIndex(const std::string& id);

public:
    static constexpr const unsigned MAX_INDEX = -1;

    std::vector<std::string> nodes;
    std::vector<std::string> channels;
    std::vector<unsigned> from;
    std::vector<unsigned> to;
    std::vector<unsigned> Tx;
    std::vector<unsigned> Rx;
};

#endif
