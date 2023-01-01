/**
 * @file channel_graph.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Implementation of Channel_Graph Class
 * @version 0.1.0
 * @date 2022-10-17
 *
 * @copyright Copyright (c) 2022-2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#include "export/channel_graph.h"

bool Channel_Graph::addChannel(const std::string& id, const std::string& from_, const std::string& to_) {
    unsigned from_i = nodeIndex(from_);
    unsigned to_i   = nodeIndex(to_);
    if (from_i == MAX_INDEX || to_i == MAX_INDEX) return false;
    from.push_back(from_i);
    to.push_back(to_i);
    channels.push_back(id);
    return true;
}

unsigned Channel_Graph::nodeIndex(const std::string& id) const {
    for (unsigned i = 0; i != nodes.size(); ++i) {
        if (nodes[i] == id) return i;
    }
    return MAX_INDEX;
}

unsigned Channel_Graph::channelIndex(const std::string& id) const {
    for (unsigned i = 0; i != channels.size(); ++i) {
        if (channels[i] == id) return i;
    }
    return MAX_INDEX;
}

bool Channel_Graph::arrange() {
    // Here we only allow one transmitter and receiver.
    if (Tx.size() > 1) return false;
    if (Rx.size() > 1) return false;
    // If no transmitter or receiver is defined,
    // it is also an error.
    if (Tx.empty() || Rx.empty()) return false;
    try {
        _formPaths({});
        if (paths.empty()) {
            std::cerr << "[mmcesim] export $ ERROR: There is no valid path in the cascaded channel.\n";
            return false;
        } else return true;
    } catch (...) {
        std::cerr << "[mmcesim] export $ ERROR: There are loops in the cascaded channel.\n";
        return false;
    }
}

void Channel_Graph::_formPaths(const std::vector<unsigned>& path) {
    if (path.empty()) {
        for (unsigned i = 0; i != channels.size(); ++i) {
            if (from[i] == Tx[0]) _formPaths({i});
        }
    } else {
        unsigned frontier = to[*(path.end() - 1)];
        for (unsigned i = 0; i != channels.size(); ++i) {
            if (from[i] == frontier) {
                if (contains(path, i)) throw("Loop in the path!");
                auto _path = path;
                _path.push_back(i);
                _formPaths(_path);
            }
        }
    }
}
