package com.webianks.bluechat.list

import android.content.Context
import android.widget.ArrayAdapter
import com.webianks.bluechat.ModeButton
import android.graphics.Color
import android.view.LayoutInflater
import android.view.View
import android.view.ViewGroup
import android.widget.ImageView
import android.widget.TextView
import android.widget.Toast
import com.webianks.bluechat.ChatFragment
import kotlinx.android.synthetic.main.activity_main.*
import com.webianks.bluechat.R
import java.util.*

class ModeAdapter (items: ArrayList<ModeButton>, ctx: Context):
    ArrayAdapter <ModeButton>(ctx, R.layout.mode, items){

    private class ModeItemViewHolder {
        internal var title: TextView? = null
        internal var nummer: TextView? =null
    }

    override fun getView(i: Int, view: View?, viewGroup: ViewGroup): View {


        var view = view

        val viewHolder: ModeItemViewHolder

        if (view == null) {
            val inflater = LayoutInflater.from(context)
            view = inflater.inflate(R.layout.mode, viewGroup, false)

            viewHolder = ModeItemViewHolder()
            viewHolder.title = view!!.findViewById<View>(R.id.title) as TextView
            viewHolder.nummer =view!!.findViewById<View>(R.id.nummer) as TextView

        } else {
            //no need to call findViewById, can use existing ones from saved view holder
            viewHolder = view.tag as ModeItemViewHolder
        }

        val modeButton = getItem(i)
        viewHolder.title!!.text = modeButton!!.modeName
        viewHolder.nummer!!.text = modeButton!!.modeid.toString()
        //shows how to handle events of views of items
//        viewHolder.title!!.setOnClickListener {
//            Toast.makeText(context, "Clicked image of " + modeButton!!.modeid,
//                    Toast.LENGTH_SHORT).show()
//
//
//        }

        view.tag = viewHolder

        return view
    }


}
