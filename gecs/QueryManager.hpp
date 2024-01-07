//
// Created by gaetz on 07/01/24.
//

#ifndef GECS_QUERYMANAGER_HPP
#define GECS_QUERYMANAGER_HPP

#include <ranges>
#include "Types.hpp"
#include "Components.hpp"

namespace gecs {

    class QueryManager {
    public:

        template <typename... ComponentTypes>
        vector<CompArchIdAndCol> GetRelevantArchetypesAndCols() {
            // Retrieve component ids from template
            vector<ComponentId> componentIds = ToComponentIds<ComponentTypes...>();
            // Create the minimal archetype id (bitset) we need
            // to find our archetypes and component cols
            std::bitset<32> pattern;
            for (const auto componentId : componentIds) {
                pattern.flip(static_cast<i32>(componentId));
            }

            return GetComponentsWithArchsCols(std::move(componentIds), pattern);
        }

        template<typename T>
        vector<T> QuerySingleComp(const vector<CompArchIdAndCol>& filterMaterial) {

            ComponentId componentId = ToComponentId<T>();
            vector<CompArchIdAndCol> compFilter = FilterForOneComponent(filterMaterial, componentId);

            vector<T> result;
            for (const auto& compArchCol : compFilter) {
                const Column& column = GetColumn(compArchCol.archId, compArchCol.columnIndex);
                const size_t count = column.Count();
                for (u32 i = 0; i < count; ++i) {
                    result.push_back(column.GetRowConst<T>(i));
                }
            }

            return result;
        }

        template<typename... ComponentTypes>
        std::tuple<vector<Id>, vector<ComponentTypes>...> Query() {
            vector<CompArchIdAndCol> filterMaterial = GetRelevantArchetypesAndCols<ComponentTypes...>();
            if (filterMaterial.empty()) return {};

            // We want relevant entities in the same order as the query

            // Take all entities, filter them by archetype and then sort them by row
            vector<IdArchRow> entitiesWithArchRows = GetEntitiesWithArchsRows();

            // Get relevant archetypes
            vector<CompArchIdAndCol> compFilter = FilterForOneComponent(filterMaterial, filterMaterial[0].componentId);

            auto filterLambda = [&compFilter](IdArchRow &a) {
                return std::find_if(compFilter.begin(), compFilter.end(), [&a](CompArchIdAndCol &filter) {
                    return filter.archId == a.archId;
                }) != compFilter.end();
            };
            auto sortLambda = [=](IdArchRow a, IdArchRow b) {
                return a.archId.to_ulong() < b.archId.to_ulong() ||
                       (a.archId.to_ulong() == b.archId.to_ulong() && a.row < b.row);
            };

            // Filter entities by archetype existing in compFilter
            //   entitiesWithArchRows = entitiesWithArchRows | std::views::filter();


            /// TODO Sort after filtering

            // Sort entities by archetype and row
            std::sort(entitiesWithArchRows.begin(), entitiesWithArchRows.end(), sortLambda);

            vector<Id> entities;
            for (const auto &entity: entitiesWithArchRows | std::views::filter(filterLambda)) {
                entities.push_back(entity.id);
            }

            // Get relevant component values in a tuple shape
            return make_tuple(entities, QuerySingleComp<ComponentTypes>(filterMaterial)...);
        }

    private:
        vector<IdArchRow> GetEntitiesWithArchsRows();
        vector<CompArchIdAndCol> GetComponentsWithArchsCols(vector<ComponentId>&& componentIds, std::bitset<32> pattern);
        Column GetColumn(ArchetypeId archId, size_t column);
        vector<CompArchIdAndCol> FilterForOneComponent(const vector<CompArchIdAndCol>& filterMaterial, ComponentId componentId);

    };
}
#endif //GECS_QUERYMANAGER_HPP
