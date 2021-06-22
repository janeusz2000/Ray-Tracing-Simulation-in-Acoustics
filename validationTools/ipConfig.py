local_ip = None
with open('/etc/resolv.conf', 'r') as f:
    for line in reversed(f.readlines()):
        keyword = 'nameserver'
        local_ip = line[line.find(keyword) + len(keyword) + 1:]
        break

if __name__ == "__main__":
    print(local_ip)
