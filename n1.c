
#include <linux/module.h>

#include <linux/netdevice.h>
#include <linux/etherdevice.h>
#include <net/dst.h>


static void send_100_zeros(struct net_device *dev)
{
  unsigned char bb[100];
  struct sk_buff *skb = dev_alloc_skb(100+2);
  memset(bb, 0, 100);
  memcpy(skb_put(skb, 100), bb, 100);
  skb->protocol = eth_type_trans(skb, dev);
  skb->dev = dev;
  
  netif_rx(skb);

}

static netdev_tx_t t1_xmit(struct sk_buff *skb,
                           struct net_device *dev)
{
  printk(KERN_INFO ">>> xmit!\n");

  dev_kfree_skb(skb);


  //skb_dst_force(skb);
  //skb->protocol = eth_type_trans(skb, dev);
  //netif_rx(skb);

  send_100_zeros(dev);

  return NETDEV_TX_OK;
}

static int t1_init(struct net_device *dev)
{
  printk(KERN_INFO ">>> init\n");
  netif_carrier_off(dev);
  return 0;
}

static int t1_open(struct net_device *dev)
{
  printk(KERN_INFO ">>> open\n");
  netif_carrier_on(dev);
  return 0;
}

static int t1_close(struct net_device *dev)
{
  printk(KERN_INFO ">>> close\n");
  netif_carrier_off(dev);
  return 0;
}


static const struct net_device_ops t1_netdev_ops = {
  .ndo_open  = t1_open,
  .ndo_stop  = t1_close,
  .ndo_init  = t1_init,
  .ndo_start_xmit= t1_xmit,
};


static u32 always_on(struct net_device *dev)
{
  return 1;
}

static const struct ethtool_ops t1_ethtool_ops = {
  .get_link = always_on,
};

static void t1_setup(struct net_device *dev)
{
  printk(KERN_INFO ">>> setup!\n");

  ether_setup(dev);

  dev->mtu = 1200;

  dev->ethtool_ops = &t1_ethtool_ops;
  dev->netdev_ops = &t1_netdev_ops;

  dev->flags |= IFF_NOARP;
  dev->priv_flags |= IFF_LIVE_ADDR_CHANGE | IFF_NO_QUEUE;
  dev->hw_features |= dev->features;
  dev->hw_enc_features |= dev->features;
}

static struct rtnl_link_ops t1_link_ops __read_mostly = {
   .kind                   = "tx",
   .priv_size              = 100,
   .setup                  = t1_setup,
};

int init_module()
{
  printk(KERN_INFO ">>> start\n");
  rtnl_link_register(&t1_link_ops);

  return 0;
}

void cleanup_module()
{
  printk(KERN_INFO ">>> done\n");
  rtnl_link_unregister(&t1_link_ops);
}

MODULE_LICENSE("GPL");

