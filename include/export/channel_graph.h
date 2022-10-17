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
#include <iostream>
#include "utils.h"

class Channel_Graph {
public:
    bool addChannel(const std::string& id, const std::string& from, const std::string& to);

    unsigned nodeIndex(const std::string& id) const;

    unsigned channelIndex(const std::string& id) const;

    /**
     * @brief Get the 'from' node index of channel
     * 
     * @param id The channel id
     * @return (unsigned) the node index
     */
    unsigned chFromIndex(const std::string& id) const;

    /**
     * @brief Get the 'to' node index of channel
     * 
     * @param id The channel id
     * @return (unsigned) the node index
     */
    unsigned chToIndex(const std::string& id) const;

    bool arrange();

private:
    void _formPaths(const std::vector<unsigned>& path);

public:
    static constexpr const unsigned MAX_INDEX = -1;

    std::vector<std::string> nodes;
    std::vector<std::string> channels;
    std::vector<unsigned> from;
    std::vector<unsigned> to;
    std::vector<unsigned> Tx;
    std::vector<unsigned> Rx;
    std::vector<std::vector<unsigned>> paths;
};

inline unsigned Channel_Graph::chFromIndex(const std::string& id) const {
    return from[channelIndex(id)];
}

inline unsigned Channel_Graph::chToIndex(const std::string& id) const {
    return to[channelIndex(id)];
}

#endif
