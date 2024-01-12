/**
 * @file channel_graph.cpp
 * @author Wuqiong Zhao (wqzhao@seu.edu.cn)
 * @brief Implementation of Channel_Graph Class
 * @version 0.2.2
 * @date 2023-04-20
 *
 * @copyright Copyright (c) 2022-2023 Wuqiong Zhao (Teddy van Jerry)
 *
 */

#include "export/channel_graph.h"

bool Channel_Graph::addChannel(const std::string& id, const std::string& from_, const std::string& to_) {
    auto from_i = nodeIndex(from_);
    auto to_i   = nodeIndex(to_);
    if (from_i == MAX_INDEX || to_i == MAX_INDEX) return false;
    from.push_back(from_i);
    to.push_back(to_i);
    channels.push_back(id);
    _log.info() << "Add Channel: [" << id << "] " << from_i << " -> " << to_i << std::endl;
    return true;
}

size_t Channel_Graph::nodeIndex(const std::string& id) const {
    for (size_t i = 0; i != nodes.size(); ++i) {
        if (nodes[i] == id) return i;
    }
    return MAX_INDEX;
}

size_t Channel_Graph::channelIndex(const std::string& id) const {
    for (size_t i = 0; i != channels.size(); ++i) {
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
    _log.info() << "Channel Graph Tx: " << Tx[0] << std::endl;
    _log.info() << "Channel Graph Rx: " << Rx[0] << std::endl;
    try {
        _formPaths({});
        if (paths.empty()) {
            std::cerr << "[mmcesim] export $ ERROR: There is no valid path in the cascaded channel." << std::endl;
            _log.err() << "There is no valid path in the cascaded channel." << std::endl;
            return false;
        } else return true;
    } catch (...) {
        std::cerr << "[mmcesim] export $ ERROR: There are loops in the cascaded channel." << std::endl;
        _log.err() << "There are loops in the cascaded channel." << std::endl;
        return false;
    }
}

void Channel_Graph::_formPaths(const std::vector<size_t>& path) {
    if (path.empty()) {
        for (size_t i = 0; i != channels.size(); ++i) {
            if (contains(Tx, from[i])) _formPaths({ i });
        }
    } else {
        auto frontier = to[*(path.end() - 1)];
        // It forms a valid path when it reaches the Rx.
        if (contains(Rx, frontier)) paths.push_back(path);
        for (size_t i = 0; i != channels.size(); ++i) {
            if (from[i] == frontier) {
                if (contains(path, i)) throw("Loop in the path!");
                auto _path = path;
                _path.push_back(i);
                _formPaths(_path);
            }
        }
    }
}

void Channel_Graph::_validatePaths() {
    // Use the reversed order here since I may remove elements in the loop.
    // std::cerr << "before path num: " << paths.size() << std::endl;
    for (size_t i = paths.size(); i != 0; --i) {
        if (auto&& path = paths[i - 1]; path.empty() || !contains(Rx, to[*(path.end() - 1)])) {
            // remove the path from paths
            paths.erase(paths.begin() + i - 1);
        }
        // possibly give a warning message
    }
    // std::cerr << "after path num: " << paths.size() << std::endl;
}
