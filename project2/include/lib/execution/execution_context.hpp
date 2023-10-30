namespace execution {
/**
 * @brief The execution context should have the 'LIVE' and 'SIM' version, for
 * 'LIVE', it should do some risk check or having some execution algo and send
 * the order to some external order entry management system (handling the
 * exchange protocol), for 'SIM', it should utilize the exchange simulator in
 * the library, and send order to the matching engine of the simulator
 *
 */
class ExecutionContext {};
} // namespace execution
