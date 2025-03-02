/**
 * @file         liblava/engine/property.hpp
 * @brief        Props master
 * @authors      Lava Block OÜ and contributors
 * @copyright    Copyright (c) 2018-present, MIT License
 */

#pragma once

#include "liblava/file/file_utils.hpp"
#include "liblava/file/json.hpp"
#include "liblava/frame/argh.hpp"
#include "liblava/fwd.hpp"

namespace lava {

/**
 * @brief Props master
 */
struct property : configurable {
    /// Engine
    engine* context = nullptr;

    /**
     * @brief Prop
     */
    struct prop {
        /// Map of props
        using map = std::map<string, prop>;

        /**
         * @brief Construct a new prop
         * @param filename    File name of prop
         */
        prop(string_ref filename)
        : filename(filename) {}

        /// File name of prop
        string filename;

        /// File data of prop
        file_data data;
    };

    /**
     * @brief Add a prop
     * @param name        Name of prop
     * @param filename    File name of prop
     */
    void add(string_ref name,
             string_ref filename);

    /**
     * @brief Remove a prop
     * @param name    Name of prop
     */
    void remove(string_ref name);

    /**
     * @brief Install a prop (add + load)
     * @param name        Name of prop
     * @param filename    File name of prop
     * @return Install was successful or failed
     */
    bool install(string_ref name,
                 string_ref filename);

    /**
     * @brief Get prop data
     * @param name      Name of prop
     * @return cdata    Prop const data
     */
    cdata operator()(string_ref name);

    /**
     * @brief Get file name of prop
     * @param name           Name of prop
     * @return string_ref    File name
     */
    string_ref get_filename(string_ref name) const {
        return map.at(name).filename;
    }

    /**
     * @brief Set filename of prop
     * @param name        Name of prop
     * @param filename    File name
     */
    void set_filename(string_ref name,
                      string_ref filename) {
        map.at(name).filename = filename;
    }

    /**
     * @brief Check if prop exists
     * @param name      Name of prop to check
     * @return Prop exists or not
     */
    bool exists(string_ref name) const {
        return map.count(name);
    }

    /**
     * @brief Check if prop data is empty
     * @param name      Name of prop
     * @return Prop data is empty or not
     */
    bool empty(string_ref name) const {
        return map.at(name).data.ptr == nullptr;
    }

    /**
     * @brief Load prop data (reload if loaded)
     * @param name      Name of prop
     * @return Load was successful or failed
     */
    bool load(string_ref name);

    /**
     * @brief Unload prop data
     * @param name      Name of prop
     */
    void unload(string_ref name) {
        map.at(name).data = {};
    }

    /**
     * @brief Load all prop data (reload if loaded)
     * @return Load was successful or failed
     */
    bool load_all();

    /**
     * @brief Unload all prop data
     */
    void unload_all() {
        for (auto& [name, prop] : map)
            prop.data = {};
    }

    /**
     * @brief Check whether all props are available
     * @return All props are there or are missing (see log)
     */
    bool check();

    /**
     * @brief Parse prop overloads
     * @param cmd_line    Command line arguments
     */
    void parse(cmd_line cmd_line);

    /**
     * @brief Clear all props
     */
    void clear() {
        map.clear();
    }

    /**
     * @brief Get all props
     * @return prop::map const&    Map of props
     */
    prop::map const& get_all() const {
        return map;
    }

    /// @see configurable::set_json
    void set_json(json_ref j) override;

    /// @see configurable::get_json
    json get_json() const override;

private:
    /// Map of props
    prop::map map;
};

} // namespace lava
