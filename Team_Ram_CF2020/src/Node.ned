//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
// 
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Lesser General Public License for more details.
// 
// You should have received a copy of the GNU Lesser General Public License
// along with this program.  If not, see http://www.gnu.org/licenses/.
// 

package mesh3;

//
// TODO auto-generated module
//
simple Node
{
    parameters:
        double delay_prob = default(0.5);
        double delay_lambda = default(0.5);
        double loss_prob = default(0.5);
        double dup_prob = default(0.5);
        double mod_prob = default(0.5);
        int max_seq = default(3);
        int timer_delay = default(7);
        int send_poll = default(1);
    gates:
        input ins[];
        output outs[];
        input pairer @directIn;
}