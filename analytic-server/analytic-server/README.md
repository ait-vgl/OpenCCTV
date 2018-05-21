# Analytic process (Analytic instance)
The Analytic Starter starts an Analytic process / Analytic instance on the system when the OpenCCTV Server sends an Analytic start request. This Analytic Runner is that daemon process, started by the Analytic Starter.

Analytic Runner loads up required video analytic plugin (shared library) and runs it. Input images are fed into the running video analytic and analytic results (output from the analytic for each input image) are sent to the OpenCCTV server.
