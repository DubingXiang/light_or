/*
 * @Author: dubing.xiang
 * @Date: 2022-07-23 15:30:07
 * @Last Modified by: dubing.xiang
 * @Last Modified time: 2022-07-23 15:30
 * @Description:
 */

#ifndef LIGHT_OR_VRP_SOLUTION_H_
#define LIGHT_OR_VRP_SOLUTION_H_
#include <unordered_map>
#include <memory>
#include <light_or/model/solution_def.h>
#include <light_or/model/solution_encodings/scp_solution.h>

namespace light_or::vrp {
class Demand {
 private:
  std::map<std::string, int> _resource_demand;
};
class Capacity {
 private:
  std::map<std::string, int> _resource_capacity;
};
class Location {
 public:
};

class Vehicle {
 public:
  DECLARE_COLUMNID_PROPERTY();

 private:
  Capacity _capacity;
};

class Customer {
 public:
  DECLARE_NODEID_PROPERTY();

 private:
  Location _location;
  Demand _demand;
};

class VRPSolution : public SCPSolution {
 public:
  Vehicle* VehicleOrNull(const ColumnId& id);
  Customer* CustomerOrNull(const NodeId& id);
  bool AddVehicle(std::unique_ptr<Vehicle> vehicle);
  bool AddCustomer(std::unique_ptr<Customer> customer);

 private:
  std::unordered_map<ColumnId, std::unique_ptr<Vehicle>> _vehicles;
  std::unordered_map<NodeId, std::unique_ptr<Customer>> _customers;
};

}  // namespace light_or::vrp

#endif