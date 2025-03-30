from scapy.all import *
import time

# Interfaces connected to the switch
iface_in = "enp0s1"
iface_out = "enp0s2"

# Packet counters
total_packets = 0
forwarding_errors = 0

def send_and_receive():
    global total_packets, forwarding_errors

    # Create a test packet
    packet = Ether(dst="ff:ff:ff:ff:ff:ff", src="00:11:22:33:44:55") / IP(dst="192.168.1.1", src="192.168.1.2") / ICMP()

    # Send the packet on iface_in
    sendp(packet, iface=iface_in, verbose=False)

    # Wait for the packet on iface_out
    response = sniff(iface=iface_out, timeout=2, count=1, lfilter=lambda x: x.haslayer(Ether) and x[Ether].src == "00:11:22:33:44:55")

    # Update counters
    total_packets += 1
    if not response:
        forwarding_errors += 1

def main():
    print("Starting MDI/MDIX forwarding test...")
    try:
        while True:
            send_and_receive()
            error_percentage = (forwarding_errors / total_packets) * 100
            print(f"Total Packets: {total_packets}, Forwarding Errors: {forwarding_errors}, Error Percentage: {error_percentage:.2f}%")
            time.sleep(1)  # Add a delay to avoid overwhelming the system
    except KeyboardInterrupt:
        print("\nTest stopped by user.")
        print(f"Final Results - Total Packets: {total_packets}, Forwarding Errors: {forwarding_errors}, Error Percentage: {(forwarding_errors / total_packets) * 100:.2f}%")

if __name__ == "__main__":
    main()